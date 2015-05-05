package continental.ingeniously.com.ingeniously.Main;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceActivity;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.List;

import continental.ingeniously.com.ingeniously.R;

public class Preferences_Activity extends Activity {

    EditText prefs_turningTime;
    EditText prefs_turningSpeed;
    Button prefs_saveButton;
    SharedPreferences sharedPreferences;


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_preferences, menu);
        return true;
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();


        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_preferences);


        sharedPreferences = getSharedPreferences("prefs", Context.MODE_PRIVATE);


        prefs_turningTime = (EditText) findViewById(R.id.prefs_turningTime);
        prefs_turningSpeed = (EditText) findViewById(R.id.prefs_turningSpeed);
        prefs_saveButton = (Button) findViewById(R.id.prefs_saveButton);


        int turnSpeed=0;
        int turnTime= 0;
        turnSpeed = sharedPreferences.getInt("TURN_SPEED",80);
        turnTime = sharedPreferences.getInt("TURN_TIME",900);

        prefs_turningTime.setText(Integer.toString(turnTime));
        prefs_turningSpeed.setText(Integer.toString(turnSpeed));
    }


    public void savePrefs(View v){

       int turnSpeed=0;
       int turnTime= 0;

        try{
            turnSpeed = Integer.parseInt(prefs_turningSpeed.getText().toString());
            turnTime = Integer.parseInt(prefs_turningTime.getText().toString());
        }catch (Exception e){
            e.printStackTrace();
            System.out.println("Preferences: You entered something that is not a number!");
        }

        SharedPreferences.Editor editor= sharedPreferences.edit();

        editor.putInt("TURN_SPEED", turnSpeed);
        editor.putInt("TURN_TIME", turnTime);
        Toast.makeText(this,"Saved",Toast.LENGTH_SHORT).show();
        System.out.println("Saved: "+turnSpeed+", "+turnTime);
       // editor.commit();
        editor.apply();

    }


}


/*extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_preferences);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_preferences, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
*/