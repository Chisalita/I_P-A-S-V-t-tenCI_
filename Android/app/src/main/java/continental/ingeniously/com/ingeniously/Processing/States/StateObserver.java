package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolCommand;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/29/2015.
 */
public interface StateObserver {
    public void stateChanged(States newState);

    public void sendCommand(ProtocolCommand command);
    void forceResponseArrived_DEBUG(ProtocolResponse response);


}
