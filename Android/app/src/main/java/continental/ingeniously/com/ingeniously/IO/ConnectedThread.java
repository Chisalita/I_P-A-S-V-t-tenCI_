package continental.ingeniously.com.ingeniously.IO;

import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

import continental.ingeniously.com.ingeniously.Main.Codes;

/**
 * Created by chisa_000 on 2/28/2015.
 */
public class ConnectedThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private final Handler mainHandler;
    private byte BUFFER[] = new byte[1024]; //12 e provizoriu...
    private int bytesInBuffer = 0;
    private final int no_of_sensors = 12;

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
            Log.d("ConnectedThread", "Unable to get IO streams!");
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

                try {
                    System.arraycopy(buffer, 0, BUFFER, bytesInBuffer, bytes); //copy to the biger buffer
                } catch (Exception e) {
                    e.printStackTrace();
                    System.out.println("ConnectedThread: out of memory for the buffer!!");
                }
                bytesInBuffer += bytes;

                int i;
                // send the packs formed in the buffer
                for (i = 0; i < (bytesInBuffer / no_of_sensors); i++) { //12 e provizoriu..

                    // Arrays.copyOfRange( buffer, i*12, (i+1)*12);
                    // Send the obtained bytes to the UI activity
                    mainHandler.obtainMessage(Codes.MESSAGE_READ, no_of_sensors, -1, Arrays.copyOfRange(BUFFER, i * no_of_sensors, (i + 1) * no_of_sensors)).sendToTarget();
                }
                //i e deja incrementat
                //shift the buffer to left to start from 0

                try {
                    System.arraycopy(BUFFER, i * 13, BUFFER, 0, bytesInBuffer - (i * no_of_sensors));
                } catch (Exception e) {
                    e.printStackTrace();
                    System.out.println("ConnectedThread: out of memory for the buffer!!");
                }



/*
                // Send the obtained bytes to the UI activity
                mainHandler.obtainMessage(Codes.MESSAGE_READ, bytes, -1, buffer).sendToTarget();
                //DEBUG!!!
                //write("OK! This is the server".getBytes());
*/
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
            //////////////
            System.out.print("CommandTRUELY: ");
            for (int i = 0; i < bytes.length; i++) {
                if (bytes[i] < 0) {
                    bytes[i] += 256;
                }
                System.out.print(String.format("%02X ", bytes[i]) + ";");
            }
            System.out.print("\n");
            ////////////////
            mmOutStream.write(bytes);
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("ConnctedThread", "Unable to send data");
        }
    }

    /* Call this from the main activity to shutdown the connection */
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("ConnctedThread", "Unable to close");
        }
    }
}
