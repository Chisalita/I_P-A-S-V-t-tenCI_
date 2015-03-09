package continental.ingeniously.com.ingeniously.IO;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import android.os.Handler;

import continental.ingeniously.com.ingeniously.Main.Codes;

/**
 * Created by chisa_000 on 2/28/2015.
 */
public class AcceptThread extends  Thread {
    private final BluetoothServerSocket mmServerSocket;
//    private final UUID MY_UUID = UUID.fromString("fa87c0d0-afac-11de-8a39-0800200c9a66");
//    private final String NAME = "com.ingeniously.continental.team10";
    private final Handler mainHandler;

    public AcceptThread(Handler handler) {
        mainHandler = handler;

        // Use a temporary object that is later assigned to mmServerSocket,
        // because mmServerSocket is final
        BluetoothServerSocket tmp = null;
        try {
            // MY_UUID is the app's UUID string, also used by the client code
            tmp = BluetoothAdapter.getDefaultAdapter().listenUsingRfcommWithServiceRecord(Codes.NAME, Codes.MY_UUID);
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("AcceptThread","Not able to listen using rfcomm!");
        }
        mmServerSocket = tmp;
    }

    public void run() {
        BluetoothSocket socket = null;
        // Keep listening until exception occurs or a socket is returned
        while (true) {
            try {
                socket = mmServerSocket.accept();
            } catch (IOException e) {
                break;
            }
            // If a connection was accepted
            if (socket != null) {




                // Do work to manage the connection (in a separate thread)
                ConnectedThread connection = new ConnectedThread(socket, mainHandler);
                connection.start();



                try {
                    mmServerSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                    Log.d("AcceptThread", "Unable to close server socket!");
                }
                break;
            }
        }
    }

    /** Will cancel the listening socket, and cause the thread to finish */
    public void cancel() {
        try {
            mmServerSocket.close();
        } catch (IOException e) { }
    }


}
