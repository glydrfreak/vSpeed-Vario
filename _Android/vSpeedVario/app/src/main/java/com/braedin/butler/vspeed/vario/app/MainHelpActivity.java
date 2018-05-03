package com.braedin.butler.vspeed.vario.app;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.webkit.WebView;
import android.widget.TextView;

import com.braedin.butler.vspeed.vario.BuildConfig;
import com.braedin.butler.vspeed.vario.R;


public class MainHelpActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mainhelp);

        TextView versionTextView = (TextView) findViewById(R.id.versionTextView);
        if (versionTextView != null) {
            versionTextView.setText("v" + BuildConfig.VERSION_NAME);
        }

        WebView infoWebView = (WebView) findViewById(R.id.infoWebView);
        if (infoWebView != null) {
            infoWebView.setBackgroundColor(Color.TRANSPARENT);
            infoWebView.loadUrl("file:///android_asset/help/app_help.html");
        }
    }
}
