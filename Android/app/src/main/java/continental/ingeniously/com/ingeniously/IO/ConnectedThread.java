package continental.ingeniously.com.ingeniously.IO;

import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import android.os.Handler;

import continental.ingeniously.com.ingeniously.Main.Codes;
import continental.ingeniously.com.ingeniously.Main.Main;

/**
 * Created by chisa_000 on 2/28/2015.
 */
public class ConnectedThread extends Thread{
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private final Handler mainHandler;


    public ConnectedThread(BluetoothSocket socket, Handler handler) {
        mainHandler = handler;
        mmSocket = socket;
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        // Get the input and output streams, using temp objects because
        // member streams are final
        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("ConnectedThread","Unable to get IO streams!");
        }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;
    }

    public void run() {
        byte[] buffer = new byte[1024];  // buffer store for the stream
        int bytes; // bytes returned from read()

        // Keep listening to the InputStream until an exception occurs
        while (true) {
            try {
                // Read from the InputStream
                bytes = mmInStream.read(buffer);
                // Send the obtained bytes to the UI activity
                mainHandler.obtainMessage(Codes.MESSAGE_READ, bytes, -1, buffer).sendToTarget();
                //DEBUG!!!
                //write("OK! This is the server".getBytes());

            } catch (IOException e) {
                e.printStackTrace();
                System.err.println("Connection Lost!");
                mainHandler.obtainMessage(Codes.CONNECTION_LOST);
                break;
            }
        }
    }

    /* Call this from the main activity to send data to the remote device */
    public void write(byte[] bytes) {
        try {
            mmOutStream.write(bytes);
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("ConnctedThread","Unable to send data");
        }
    }

    /* Call this from the main activity to shutdown the connection */
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("ConnctedThread","Unable to close");
        }
    }
}
