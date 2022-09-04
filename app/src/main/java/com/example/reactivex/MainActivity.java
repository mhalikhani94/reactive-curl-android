package com.example.reactivex;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.example.reactivex.databinding.ActivityMainBinding;
import com.example.reactivex.network.NetworkManager;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("reactivex");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
    }

    public void handlePostClicked(View view) {
        EditText editText = findViewById(R.id.PostEditText);
        TextView textView = findViewById(R.id.textView);
        NetworkManager mgr = new NetworkManager();
        String url = editText.getText().toString();
        mgr.sendPostRequest(url).subscribe(
                s -> {
                    textView.setText(s);
                },
                throwable -> {
                }
        );
//        textView.setText("Hello Form The Other Side!!!!");
    }

    public void handleGetClicked(View view) {
        EditText editText = findViewById(R.id.getEditText);
        TextView textView = findViewById(R.id.textView2);
        NetworkManager mgr = new NetworkManager();
        String url = editText.getText().toString();
        mgr.sendGetRequest(url).subscribe(
                s -> {
                    textView.setText(s);
                },
                throwable -> {
                }
        );
//        textView.setText("Hello Form Another The Other Side!!!!");

    }

    public native String stringFromJNI();
}