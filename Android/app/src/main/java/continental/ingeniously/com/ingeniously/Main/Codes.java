package continental.ingeniously.com.ingeniously.Main;

import java.util.UUID;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolCommand;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 2/28/2015.
 */
public class Codes {
    public static final int CONNECTION_ESTABLISHED = 1900;
    public static final int CONNECTION_LOST = 1901;
    public static final int MESSAGE_READ = 1902;
    public static final int REQUEST_ENABLE_BT=1001;
    public static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
    public static final String NAME = "com.ingeniously.continental.team10";
    public static final String CAR_BLUETOOTH_NAME = "TEAM10";
    public static final int MODE_NORMAL = 1800;
    public static final int MODE_SIDEWAYS = 1801;
    public static final ProtocolCommand STATUS_UPDATE_COMMAND = new ProtocolCommand((byte)0x80, (byte) 0, (byte) 0, (short) 0);
}
