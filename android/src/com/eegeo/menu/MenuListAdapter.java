// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

package com.eegeo.menu;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import com.eegeo.entrypointinfrastructure.MainActivity;
import com.eegeo.mobileexampleapp.R;
import com.eegeo.categories.CategoryResources;

import android.animation.ValueAnimator;
import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MenuListAdapter extends BaseAdapter
{
    private MainActivity m_context;
    private List<String> m_groups;
    private List<Boolean> m_groupsExpandable;
    private HashMap<String, List<String>> m_groupToChildrenMap;
    private int m_groupViewId;
    private int m_childViewId;
    private int m_menuBackgroundId;
    private int m_subMenuBackgroundId;
    private HashMap<String, Integer> m_animatedSizesMap;
    private ValueAnimator m_expandContractAnim;
    private boolean m_shouldAlignIconRight;

    private final float SubItemIndent = 22.0f;
    
    public MenuListAdapter(MainActivity context,
                           final int groupViewId,
                           final int childViewId,
                           final boolean shouldAlignIconRight,
                           final int menuBackgroundId,
                           final int subMenuBackgroundId)
    {
        m_context = context;

        m_groupViewId = groupViewId;
        m_childViewId = childViewId;
        m_animatedSizesMap = new HashMap<String, Integer>();

        m_groups = new ArrayList<String>();
        m_groupsExpandable = new ArrayList<Boolean>();
        m_groupToChildrenMap = new HashMap<String, List<String>>();

        m_shouldAlignIconRight = shouldAlignIconRight;
        m_menuBackgroundId = menuBackgroundId;
        m_subMenuBackgroundId = subMenuBackgroundId;
    }

    public boolean isAnimating()
    {
        return m_expandContractAnim != null && m_expandContractAnim.isRunning();
    }

    public void setData(
        final List<String> groups,
        final List<Boolean> groupsExpandable,
        final HashMap<String, List<String>> groupToChildren)
    {
        if(m_groups.size() != groups.size())
        {
            updateSources(groups, groupsExpandable, groupToChildren);
            forceSetAnimatedSizes();
            notifyDataSetChanged();
        }
        else
        {
            updateAndAnimateSources(groups, groupsExpandable, groupToChildren);
        }
    }

    public void updateSources(
        final List<String> groups,
        final List<Boolean> groupsExpandable,
        final HashMap<String, List<String>> groupToChildren)
    {
        m_groups = groups;
        m_groupsExpandable = groupsExpandable;
        m_groupToChildrenMap = groupToChildren;
    }

    private void forceSetAnimatedSizes()
    {
        for(int i = 0; i < m_groups.size(); i++)
        {
            String key = m_groups.get(i);
            m_animatedSizesMap.put(key, m_groupToChildrenMap.get(key).size());
        }
    }

    public void setAnimatedGroupSize(String groupName, int size)
    {
        if(!m_animatedSizesMap.containsKey(groupName))
        {
            throw new IllegalArgumentException("No group named " + groupName + " found!");
        }

        if(m_animatedSizesMap.get(groupName) != size)
        {
            m_animatedSizesMap.put(groupName, size);
            notifyDataSetChanged();
        }
    }

    private void updateAndAnimateSources(
        final List<String> groups,
        final List<Boolean> groupsExpandable,
        final HashMap<String, List<String>> groupToChildren)
    {
        boolean anySizeChanges = false;

        // Check each group to see if it has changed in size.
        for(int i = 0; i < m_groups.size(); i++)
        {
            final int groupIndex = i;
            final String groupName = m_groups.get(groupIndex);
            int currentSize = m_groupToChildrenMap.get(groupName).size();

            if(!groupToChildren.containsKey(groupName))
            {
                throw new IllegalArgumentException("New data source missing group '" + groupName + "'. Cannot animate sizes!");
            }
            int targetSize = groupToChildren.get(groupName).size();

            if(currentSize == targetSize)
            {
                continue;
            }

            anySizeChanges = true;

            // If so, animate them to the new size.
            m_expandContractAnim = ValueAnimator.ofInt(currentSize, targetSize);
            m_expandContractAnim.setDuration(Math.abs((targetSize - currentSize) * 30));
            m_expandContractAnim.addUpdateListener(new MenuSectionAnimatorUpdateListener(this, groupName));

            // If contracting, update sources at the end of the animation, otherwise update it at the beginning.
            if(targetSize < currentSize)
            {
                m_expandContractAnim.addListener(new MenuDelayedSourceUpdateAnimatorListener(this, groups, groupsExpandable, groupToChildren));
            }
            else
            {
                updateSources(groups, groupsExpandable, groupToChildren);
            }

            m_expandContractAnim.start();
        }

        // If there were no size changes, don't animate anything and just refresh the data sources.
        if(!anySizeChanges)
        {
            updateSources(groups, groupsExpandable, groupToChildren);
            notifyDataSetChanged();
        }
    }

    @Override
    public int getCount()
    {
        int count = 0;
        for(int groupIndex = 0; groupIndex < m_groups.size(); groupIndex++)
        {
            String key = m_groups.get(groupIndex);
            count += m_animatedSizesMap.get(key);
        }
        return count;
    }

    @Override
    public Object getItem(int index)
    {
        int count = 0;
        for(int groupIndex = 0; groupIndex < m_groups.size(); groupIndex++)
        {
            String groupName = m_groups.get(groupIndex);
            List<String> children = m_groupToChildrenMap.get(groupName);
            if(children.size() == 0)
            {
                continue;
            }

            for(int childIndex = 0; childIndex < m_animatedSizesMap.get(groupName); childIndex++)
            {
                if(count == index)
                {
                    return children.get(childIndex);
                }

                count++;
            }
        }
        return "";
    }

    @Override
    public long getItemId(int index)
    {
        return index;
    }

    public int getSectionIndex(int index)
    {
        int count = 0;
        for(int groupIndex = 0; groupIndex < m_groups.size(); groupIndex++)
        {
            String groupName = m_groups.get(groupIndex);
            List<String> children = m_groupToChildrenMap.get(groupName);
            for (int childIndex = 0; childIndex < children.size(); ++childIndex)
            {
                if (count == index)
                {
                    return groupIndex;
                }
                count++;
            }
        }
        return -1;
    }

    public int getItemIndex(int index)
    {
        int count = 0;
        for(int groupIndex = 0; groupIndex < m_groups.size(); groupIndex++)
        {
            String groupName = m_groups.get(groupIndex);
            List<String> children = m_groupToChildrenMap.get(groupName);
            for (int childIndex = 0; childIndex < children.size(); ++childIndex)
            {
                if (count == index)
                {
                    return childIndex;
                }
                count++;
            }
        }
        return -1;
    }

    private boolean isHeader(int index)
    {
        int groupIndex = getGroupIndexForViewIndex(index);
        int placeInGroup = getPlaceInGroup(index);

        if(!m_groupsExpandable.get(groupIndex))
        {
            return true;
        }
        return placeInGroup == 0;
    }

    @Override
    public View getView(int index, View reusableView, ViewGroup parent)
    {
        final String json = (String)getItem(index);
        
        final boolean isHeader = isHeader(index);

        final int viewId = isHeader ? m_groupViewId : m_childViewId;
        if(reusableView == null || (Integer)reusableView.getTag() != viewId)
        {
            LayoutInflater inflater = (LayoutInflater)m_context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            reusableView = inflater.inflate(viewId, null);
            reusableView.setTag((Integer)viewId);
        }

        try
        {
            JSONObject data = new JSONObject(json);

            TextView nameLabel = (TextView)reusableView.findViewById(R.id.menu_list_item_name);
            nameLabel.setText(data.getString("name"));

            int margin = 0;
            ImageView icon = null;
            
            if(!isHeader)
            {
            	icon = (ImageView)reusableView.findViewById(R.id.menu_list_item_icon);
            	icon.setImageResource(CategoryResources.getSmallIconForResourceName(m_context, data.getString("icon")));

            	margin = icon.getLayoutParams().width + 10;
            }

            if (m_shouldAlignIconRight)
            {
                if(icon != null)
                {
	                RelativeLayout.LayoutParams iconParams = new RelativeLayout.LayoutParams(icon.getLayoutParams());
	                iconParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, RelativeLayout.TRUE);
	                icon.setLayoutParams(iconParams);
                }

                RelativeLayout.LayoutParams textParams = new RelativeLayout.LayoutParams(nameLabel.getLayoutParams());
                nameLabel.setGravity(Gravity.CENTER_VERTICAL|Gravity.RIGHT);
                textParams.setMargins(0, 0, margin, 0);
                nameLabel.setLayoutParams(textParams);
            }
        }
        catch(JSONException e)
        {
            Log.e("Eegeo", "MenuListAdapter: Failed to read json data object: " + e.getMessage());
        }

        if(!isHeader)
        {
            float scale = m_context.getResources().getDisplayMetrics().density;
            reusableView.setPadding(
                !m_shouldAlignIconRight ? (int)(SubItemIndent * scale + 0.5f) : 0,
                0,
                m_shouldAlignIconRight ? (int)(SubItemIndent * scale + 0.5f) : 0,
                0);
        }
        else
        {
            RelativeLayout openableArrow = (RelativeLayout)reusableView.findViewById(R.id.menu_list_openable_shape);
            int groupIndex = getGroupIndexForViewIndex(index);

            if(m_groupsExpandable.get(groupIndex))
            {
                RelativeLayout.LayoutParams openableArrowParams = new RelativeLayout.LayoutParams(openableArrow.getLayoutParams());

                if(m_shouldAlignIconRight)
                {
                    openableArrowParams.addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE);
                    openableArrow.setRotation(180);
                    openableArrowParams.leftMargin = m_context.dipAsPx(20);
                }
                else
                {
                    openableArrowParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, RelativeLayout.TRUE);
                    openableArrow.setRotation(0);
                    openableArrowParams.rightMargin = m_context.dipAsPx(20);
                }

                String groupName = m_groups.get(groupIndex);
                if(m_animatedSizesMap.get(groupName) > 1)
                {
                    openableArrow.setRotation(270);
                }

                openableArrowParams.addRule(RelativeLayout.CENTER_VERTICAL, RelativeLayout.TRUE);
                openableArrow.setLayoutParams(openableArrowParams);
            }
            else
            {
                openableArrow.setVisibility(View.GONE);
            }
        }
        
        View backgroundContainer = reusableView.findViewById(R.id.menu_list_item_background_container);
        if(isHeader)
        {
        	backgroundContainer.setBackgroundDrawable(m_context.getResources().getDrawable(m_menuBackgroundId));
        }
        else
        {
        	backgroundContainer.setBackgroundDrawable(m_context.getResources().getDrawable(m_subMenuBackgroundId));	
        }
        

        View dividerContainer = reusableView.findViewById(R.id.menu_list_divider_container);
        RelativeLayout.LayoutParams dividerContainerParams = new RelativeLayout.LayoutParams(dividerContainer.getLayoutParams());
        if(isHeader)
        {
        	dividerContainer.setBackgroundColor(m_context.getResources().getColor(R.color.menu_separator));
        	dividerContainerParams.leftMargin = m_context.dipAsPx(22);
        }
        else
        {
        	dividerContainer.setBackgroundColor(m_context.getResources().getColor(R.color.submenu_separator));
        	dividerContainerParams.leftMargin = m_context.dipAsPx(36);
        }
        dividerContainerParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);
        dividerContainer.setLayoutParams(dividerContainerParams);
        
        
        return reusableView;
    }

    private int getGroupIndexForViewIndex(int index)
    {
        int count = 0;
        for(int groupIndex = 0; groupIndex < m_groups.size(); groupIndex++)
        {
            String groupName = m_groups.get(groupIndex);
            for(int childIndex = 0; childIndex < m_animatedSizesMap.get(groupName); childIndex++)
            {
                if(count == index)
                {
                    return groupIndex;
                }
                count++;
            }
        }
        return -1;
    }

    public int getPlaceInGroup(int index)
    {
        int count = 0;
        for(int groupIndex = 0; groupIndex < m_groups.size(); groupIndex++)
        {
            String groupName = m_groups.get(groupIndex);
            for(int childIndex = 0; childIndex < m_animatedSizesMap.get(groupName); childIndex++)
            {
                if(count == index)
                {
                    return childIndex;
                }
                count++;
            }
        }
        return 0;
    }

}
