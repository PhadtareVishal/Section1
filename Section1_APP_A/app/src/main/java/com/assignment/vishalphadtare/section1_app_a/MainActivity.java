package com.assignment.vishalphadtare.section1_app_a;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity
{
    Button sendBtn;
    TextView dataTxtView;


    private static final String KEY_TASK = "request_task";
    private static final String KEY_APP = "respond_to";
    private static final String KEY_PARAM1 = "request_param1";
    private static final String KEY_PARAM2 = "request_param2";

    private static final String VALUE_TASK_ECHO = "get_echo";

    BroadcastReceiver m_receiver = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            configureReceiver();
            dataTxtView = (TextView)findViewById(R.id.editText);
            sendBtn     = (Button) findViewById(R.id.button_send);
        }
        catch (Exception e){
            e.printStackTrace();
        }

        dataTxtView.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }
            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                    String userInput = dataTxtView.getText().toString().trim();
                    sendBtn.setEnabled(!userInput.isEmpty());
            }
            @Override
            public void afterTextChanged(Editable editable) {
            }
        });
    }

    public void getEcho(View view){
        final Intent intent=new Intent();
        intent.setAction("com.assignment.vishalphadtare.section1_app_b");

        intent.putExtra(KEY_APP, getApplicationContext().getPackageName()); // send broad cast to me again
        intent.putExtra(KEY_TASK,VALUE_TASK_ECHO);
        intent.putExtra(KEY_PARAM1, dataTxtView.getText().toString());

        intent.addFlags(Intent.FLAG_INCLUDE_STOPPED_PACKAGES);
        sendBroadcast(intent);
    }

    private void configureReceiver(){
        IntentFilter filter = new IntentFilter();
        filter.addAction(getApplicationContext().getPackageName());
        m_receiver = new MyResponceReceiver();
        registerReceiver(m_receiver, filter);
    }

    @Override
    protected void onDestroy() {
    super.onDestroy();
    unregisterReceiver(m_receiver);
    m_receiver = null;
}

}
