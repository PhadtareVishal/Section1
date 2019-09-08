package com.assignment.vishalphadtare.section1_app_b;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.util.logging.Handler;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */
public class MyIntentService extends IntentService {

    public native String registerFromJNI(MyIntentService activity);
    native void onInit();
    // TODO: Rename actions, choose action names that describe tasks that this
    // IntentService can perform, e.g. ACTION_FETCH_NEW_ITEMS
    public static final String ACTION_FOO = "com.assignment.vishalphadtare.section1_app_b.action.FOO";
    public static final String ACTION_BAZ = "com.assignment.vishalphadtare.section1_app_b.action.BAZ";

    /* TODO:
    TASK to be consider yet
     */
    public static final String KEY_APP = "respond_to";
    public static final String KEY_TASK = "request_task";
    public static final String EXTRA_PARAM1 = "request_param1";
    public static final String EXTRA_PARAM2 = "request_param2";

    static {
        System.loadLibrary("native-lib");
        Log.i("HERE", "System.loadLibrary");
    }
    public MyIntentService() {
        super("MyIntentService");
        Log.i("HERE", "in side MyIntentService: ");
    }

    @Override
    public void onCreate (){
        super.onCreate();
        Log.i("HERE", "MyIntentService created ");
    }

    @Override
    public void onDestroy (){
        super.onDestroy();
        Log.i("HERE", "MyIntentService Destroyed ");
    }

    public void returnValueReceived(String message)
    {
        try
        {
            Log.i("HERE", "returnValueReceived received :"+message);
        }
        catch(Exception e)
        {
            Log.i("HERE", "MyIntentService exception :"+e.toString());
        }
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionFoo(Context context, String param1, String param2) {
        Log.i("HERE", "in side startActionFoo: "+param1);
        Intent intent = new Intent(context, MyIntentService.class);
        intent.setAction(ACTION_FOO);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
    }

    /**
     * Starts this service to perform action Baz with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionBaz(Context context, String param1, String param2) {
        Intent intent = new Intent(context, MyIntentService.class);
        intent.setAction(ACTION_BAZ);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            Log.i("HERE", "in side onHandleIntent action: "+action);
            if (ACTION_FOO.equals(action)) {
                final String respondTo = intent.getStringExtra(KEY_APP);
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                final String param2 = intent.getStringExtra(EXTRA_PARAM2);
                registerFromJNI(this);
                //Log.i("HERE", "inside onHandleIntent: "+param1+" "+stringFromJNI() );
                onInit();
                //handleActionFoo(respondTo, param1, param2);
            } else if (ACTION_BAZ.equals(action)) {
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                final String param2 = intent.getStringExtra(EXTRA_PARAM2);
                handleActionBaz(param1, param2);
            }
        }
    }

    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private void handleActionFoo(String respondTo, String param1, String param2) {
        Log.i("HERE", "inside handleActionFoo: "+param1);

        Intent intent = new Intent();
        intent.setAction(respondTo.toString());
        intent.putExtra("ANSWER", param1.toString()); // Sending Param back again
        sendBroadcast(intent);
        // TODO: Handle action Foo
       // throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * Handle action Baz in the provided background thread with the provided
     * parameters.
     */
    private void handleActionBaz(String param1, String param2) {
        // TODO: Handle action Baz
        throw new UnsupportedOperationException("Not yet implemented");
    }


}
