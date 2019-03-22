package com.animeshpatni.converter;

import android.nfc.Tag;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.time.LocalDate;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private TextView h;
    private TextView ans;
    private EditText temp;
    private RadioButton r;
    private RadioButton r2;
    private int c=1;
    //private RadioGroup radioGroup;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate: " + c++ + "+++");
        setContentView(R.layout.activity_main);
        h = findViewById(R.id.history);
        h.setMovementMethod(new ScrollingMovementMethod());
    }

    public void radiocheck(View v)
    {
        String selectionText = ((RadioButton) v).getText().toString();
        Toast.makeText(this,"You have selected: "+selectionText,Toast.LENGTH_SHORT).show();
    }

    public void conversion(View v)
    {
        try {
            temp = findViewById(R.id.temp);
            double t = Double.parseDouble(temp.getText().toString());


            r = findViewById(R.id.radio1);
            r2 = findViewById(R.id.radio2);

            if (r.isChecked()) {
                t = ((t - 32.0) * (5.0 / 9.0));
            } else if (r2.isChecked()) {
                t = ((t * (9.0 / 5.0)) + 32.0);
            }
            ans = findViewById(R.id.answer);
            ans.setText(String.format("%.1f", t));

            String contemp = ans.getText().toString();
            String inputtemp = temp.getText().toString();
            String historytemp = h.getText().toString();

            if (r.isChecked()) {
                h.setText("F to C : " + inputtemp + "  ->  " + contemp + "\n" + historytemp);
            } else if (r2.isChecked()) {
                h.setText("C to F : " + inputtemp + "  ->  " + contemp + "\n" + historytemp);
            }
        }
        catch(Exception e)
        {
            Toast.makeText(this,"Please Enter A Value",Toast.LENGTH_SHORT).show();
        }
        //ans.setText(String.format("%.1f",t));
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "onStart: "+ c++ + "++++");
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "onStop: "+ c++ + "++++");
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "onPause: "+ c++ + "++++");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.d(TAG, "onRestart: "+ c++ + "++++");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume: "+ c++ + "++++");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy: "+ c++ + "++++");
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        Log.d(TAG, "onSaveInstanceState: "+ c++ + "+++++");
        super.onSaveInstanceState(outState);
        outState.putString("HISTORY", h.getText().toString());
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        Log.d(TAG, "onRestoreInstanceState: "+ c++ + "+++++");
        super.onRestoreInstanceState(savedInstanceState);
        h.setText(savedInstanceState.getString("HISTORY"));
    }
}
