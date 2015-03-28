package continental.ingeniously.com.ingeniously.Main;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import continental.ingeniously.com.ingeniously.R;

public class Starter_Activity extends ActionBarActivity {

    private TextView ParkiPos_editText;
    private TextView ExitNo_editText;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_starter_);

        ParkiPos_editText = (TextView) findViewById(R.id.ParkingPos_editText);
        ExitNo_editText = (TextView) findViewById(R.id.ExitNo_editText);

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_starter_, menu);
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

    public void ModeSelect(View v){
        switch (v.getId()){
            case R.id.Normal_radioButton:

                break;
            case R.id.Sideways_radioButton:

                break;
        }

    }

    public void Start(View v){

        System.out.println(ParkiPos_editText.getText());
        System.out.println(ExitNo_editText.getText());


    }

}
