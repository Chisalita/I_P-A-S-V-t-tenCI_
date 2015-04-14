package continental.ingeniously.com.ingeniously.IO.Protocol;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.IntentFilter;
import android.os.Handler;
import android.widget.ArrayAdapter;

import java.util.Set;

import continental.ingeniously.com.ingeniously.IO.ConnectThread;
import continental.ingeniously.com.ingeniously.Main.ApplicationBrodcastReciver;
import continental.ingeniously.com.ingeniously.Main.Codes;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public class BluetoothIO implements Protocol {

    private Context mainContext;
    private ConnectThread connection;
    private final Handler mainHandler;
    private BluetoothAdapter mBluetoothAdapter;
    private ApplicationBrodcastReciver mReciver;
    private ArrayAdapter<String> BluetoothDevicesArrayAdapter;
    private boolean isRegistered = false;


    public BluetoothIO(Context context, BluetoothAdapter ba, Handler handler){
        mainHandler = handler;
        mBluetoothAdapter  = ba;
        mReciver = new ApplicationBrodcastReciver(BluetoothDevicesArrayAdapter);
        mainContext = context;
    }


    @Override
    public ProtocolResponse getLastResponse() {
        return null;
    }

    @Override
    public ProtocolCommand getLastCommand() {
        return null;
    }

    @Override
    public void sendCommand(ProtocolCommand command) {

        if (connection != null && command != null) {
            System.out.println("Sends");

            byte cm[]=command.getBytes();

            System.out.print("Command: ");
            for(int i=0; i<cm.length; i++){
                System.out.print(cm[i]+";" );
            }
            System.out.print("\n");
            //command.setForward((byte)40);
            System.out.println("Verify: "+command.verifyCRC());

            connection.sendData(command.getBytes());
        }

    }

    @Override
    public void connect() {

        BluetoothDevice carBluetooth = getPairedDeviceByName(Codes.CAR_BLUETOOTH_NAME);
        if (carBluetooth == null) {
            System.out.println("Bluetooth not found in paired devices");
           // Toast.makeText(this, Codes.CAR_BLUETOOTH_NAME + " Bluetooth not found in paired devices", Toast.LENGTH_LONG).show();
        } else {

            if (connection != null) {
                connection.cancel();
            }
            connection = new ConnectThread(carBluetooth, mainHandler);
            connection.start();
        }


    }

    @Override
    public void disconnect() {
        if (connection != null) {
            connection.cancel();
        }

    }

    @Override
    public void clearAll() {

    }


    BluetoothDevice getPairedDeviceByName(String name) {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();

        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                //Found the desired device
                if (device.getName().contains(name)) {
                    // return the device
                    return device;
                }
            }
        }
        return null;
    }


    public void unRegisterBluetoothSearch(){
        if (isRegistered) {
            mainContext.unregisterReceiver(mReciver);
            isRegistered = false;
        }

    }

    private void SearchForBluetoothDevices() {
        // Searches for bluetooth devices


        // Register the BroadcastReceiver
        if (!isRegistered) {
            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            mainContext.registerReceiver(mReciver, filter); // Don't forget to unregister during onDestroy
            isRegistered = true;
        }

        mBluetoothAdapter.cancelDiscovery();
        mBluetoothAdapter.startDiscovery();

    }


}
