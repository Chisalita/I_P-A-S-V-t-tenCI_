package continental.ingeniously.com.ingeniously.Main;

import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.ArrayAdapter;

/**
 * Created by chisa_000 on 2/28/2015.
 */
public class ApplicationBrodcastReciver extends BroadcastReceiver {

    private ArrayAdapter<String> mArrayAdapter  = null;

    public ApplicationBrodcastReciver(ArrayAdapter<String> aA){
        mArrayAdapter = aA;
    }


    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        // When discovery finds a device
        if (BluetoothDevice.ACTION_FOUND.equals(action)) {
            // Get the BluetoothDevice object from the Intent
            BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
            // Add the name and address to an array adapter to show in a ListView
            mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            System.out.println("ApplicationBrocastReciever: found: "+device.getName() + "\n" + device.getAddress());
        }
    }

}
