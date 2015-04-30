package continental.ingeniously.com.ingeniously.IO.Protocol;

import android.content.Intent;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public interface Protocol {

    ProtocolResponse getLastResponse();
    ProtocolCommand getLastCommand();
    void sendCommand(ProtocolCommand command);
    void connect();
    void disconnect();
    void clearAll();
    void forceResponseArrived_DEBUG(ProtocolResponse response);

}
