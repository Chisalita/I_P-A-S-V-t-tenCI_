package continental.ingeniously.com.ingeniously.Main;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import continental.ingeniously.com.ingeniously.R;

public class DebugActivity extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_debug, menu);
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



    public  String fetchCommand(String command) {

        command = command.toLowerCase();
        String commands[] = command.split("\\;");

        for (int i=0 ; i <= commands.length-1 ; i++)
        {
           // System.out.println("Commands: "+commands[i]+"\n");

            if(commands[i].startsWith("move=")) {

                String tokens[] =  commands[i].split("\\=");
                String parameterList = tokens[1];
                move(parameterList);

            }else if(commands[i].startsWith("search")){

                serachForDevices();

            }else {
                System.out.println("Undefined command: \"" + commands[i] + "\" !");
            }

        }

        return command;
    }


    private  void move(String parameterList) {

        String parameters[] = parameterList.split("\\,");
        int leftOrRight,forwardOrBackward,time;
        try {
            leftOrRight = Integer.parseInt(parameters[0]);
            forwardOrBackward = Integer.parseInt(parameters[1]);
            time = Integer.parseInt(parameters[2]);
        }catch (Exception e) {
            e.printStackTrace();
            return;
        }

        if(leftOrRight > 0 ) {
            System.out.print("Moving left: " + leftOrRight);
        }else {
            System.out.print("Moving right: " + leftOrRight);
        }

        if(forwardOrBackward > 0) {
            System.out.print(" - forward: " + forwardOrBackward);
        }else {
            System.out.print(" - backward: " + forwardOrBackward);
        }

        System.out.println(" - time:" + time);

    }


    private static void serachForDevices() {
        // TODO Auto-generated method stub
        System.out.println("This is where you implement the \"search\" function!");
    }


}
