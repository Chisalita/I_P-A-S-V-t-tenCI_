package continental.ingeniously.com.ingeniously.IO;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.os.ParcelUuid;

import java.io.IOException;
import java.util.UUID;

import continental.ingeniously.com.ingeniously.Main.Codes;

/**
 * Created by chisa_000 on 3/3/2015.
 */
public class ConnectThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final BluetoothDevice mmDevice;
    private final Handler mainHandler;
    private  ConnectedThread connection;

    public ConnectThread(BluetoothDevice device, Handler handler) {
        // Use a temporary object that is later assigned to mmSocket,
        // because mmSocket is final
        BluetoothSocket tmp = null;
        mmDevice = device;
        mainHandler = handler;


        /*ParcelUuid uid[]=device.getUuids();

        if(uid !=null ) {
            for (int i = 0; i < device.getUuids().length; i++) {
                System.out.println(device.getUuids()[i]);
            }
        }*/




        //connection = new ConnectedThread(mmDevice,mainHandler);

        // Get a BluetoothSocket to connect with the given BluetoothDevice
        try {
            // MY_UUID is the app's UUID string, also used by the server code
            tmp = device.createRfcommSocketToServiceRecord(Codes.MY_UUID);
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("ConnectThread: Unable to get bluetooth socket!");
        }
        mmSocket = tmp;
    }

    public void run() {
        // Cancel discovery because it will slow down the connection
        BluetoothAdapter.getDefaultAdapter().cancelDiscovery();

        try {
            // Connect the device through the socket. This will block
            // until it succeeds or throws an exception
            mmSocket.connect();
        } catch (IOException connectException) {
            // Unable to connect; close the socket and get out
            System.err.println("Connect Thread: can not connect!");
            try {
                mmSocket.close();
            } catch (IOException closeException) {
                closeException.printStackTrace();
                System.err.println("ConnectThread: Unable to close bluetooth socket!");
            }
            return;
        }

        // Do work to manage the connection (in a separate thread)
        // manageConnectedSocket(mmSocket);
        System.out.println("Conncted!");
        mainHandler.obtainMessage(Codes.CONNECTION_ESTABLISHED, 0, -1, 0).sendToTarget();

        connection = new ConnectedThread(mmSocket,mainHandler);
        connection.start();
    }

    public void sendData(String s){

        if(connection != null){
            connection.write(s.getBytes());
        }

    }



    /** Will cancel an in-progress connection, and close the socket */
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("ConnectThread: Unable to close bluetooth socket!");
        }
    }
}
