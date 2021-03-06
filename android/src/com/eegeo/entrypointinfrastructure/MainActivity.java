// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

package com.eegeo.entrypointinfrastructure;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import com.eegeo.photos.PhotoIntentDispatcher;

public abstract class MainActivity extends Activity implements SurfaceHolder.Callback, INativeMessageRunner
{
    private PhotoIntentDispatcher m_photoIntentDispatcher;

    public MainActivity()
    {
        m_photoIntentDispatcher = new PhotoIntentDispatcher(this);
    }

    public PhotoIntentDispatcher getPhotoIntentDispatcher()
    {
        return m_photoIntentDispatcher;
    }

    // http://developer.android.com/guide/practices/screens_support.html#dips-pels
    // and http://stackoverflow.com/a/9904844
    public int pxAsDip(float px)
    {
        float density = getResources().getDisplayMetrics().density;
        int dip = (int)((px / density) + 0.5f);
        return dip;
    }

    public int dipAsPx(float dip)
    {
        float density = getResources().getDisplayMetrics().density;
        int px = (int)((dip * density) + 0.5f);
        return px;
    }

    public void dismissKeyboard(IBinder binder)
    {
        InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(binder, 0);
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
    }
    
    public void recursiveDisableSplitMotionEvents(ViewGroup group)
    {
    	group.setMotionEventSplittingEnabled(false);
    	
    	for(int i = 0; i < group.getChildCount(); i++) 
    	{
    		View child = group.getChildAt(i);
    	
    		if(child instanceof ViewGroup) 
    		{
    			recursiveDisableSplitMotionEvents((ViewGroup)child);
    		}
    	}
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        m_photoIntentDispatcher.onActivityResult(requestCode, resultCode, data);
    }

    public abstract void dispatchRevealUiMessageToUiThreadFromNativeThread(final long nativeCallerPointer);

    public abstract void dispatchUiCreatedMessageToNativeThreadFromUiThread(final long nativeCallerPointer);
}
