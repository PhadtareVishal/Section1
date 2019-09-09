package com.assignment.vishalphadtare.section1_app_b;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;


public class RequestReceiver extends BroadcastReceiver {

    /*MainActivity ma; //a reference to activity's context
    public MyReceiver(MainActivity maContext){
        ma=maContext;
    }*/

    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO: This method is called when the BroadcastReceiver is receiving
        // an Intent broadcast.
        //throw new UnsupportedOperationException("Not yet implemented");

        Log.i("HERE", "KEY_APP "+intent.getStringExtra(MyIntentService.KEY_APP).toString());
        Log.i("HERE", "KEY_TASK "+intent.getStringExtra(MyIntentService.KEY_TASK).toString());
        //Log.i("HERE", "EXTRA_PARAM1 "+intent.getStringExtra(MyIntentService.EXTRA_PARAM1).toString());
        Intent passDataIntent = new Intent(context, MyIntentService.class);
        passDataIntent.setAction("com.assignment.vishalphadtare.section1_app_b.action.FOO");
        passDataIntent.putExtra(MyIntentService.KEY_APP,        intent.getStringExtra(MyIntentService.KEY_APP).toString());
        passDataIntent.putExtra(MyIntentService.KEY_TASK,       intent.getStringExtra(MyIntentService.KEY_TASK).toString());
        passDataIntent.putExtra(MyIntentService.EXTRA_PARAM1,   intent.getStringExtra(MyIntentService.EXTRA_PARAM1).toString());


        context.startService(passDataIntent);
       // Toast.makeText(context, "Broadcast Intent Detected.",Toast.LENGTH_LONG).show();
    }
}
