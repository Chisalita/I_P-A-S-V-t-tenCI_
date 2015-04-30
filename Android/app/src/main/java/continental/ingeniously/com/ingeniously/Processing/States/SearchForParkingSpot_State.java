package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;
import continental.ingeniously.com.ingeniously.Main.Codes;

/**
 * Created by chisa_000 on 4/28/2015.
 */
public class SearchForParkingSpot_State extends State {
    StateObserver stateObserver;

    @Override
    public void process(ProtocolResponse response) {
        stateObserver.sendCommand(Codes.STATUS_UPDATE_COMMAND); //exemplu de trimitere a comenzii STATUS UPDATE dar poti crea o comanda in care sa scri ce vrei
        System.out.println("Search for parking spot state processing");
        //stateObserver.forceResponseArrived_DEBUG(new ProtocolResponse()); // asta simuleaza faptul ca ai primit un raspuns
    }

    public SearchForParkingSpot_State(StateObserver observer,int mode) {
        stateObserver = observer;
    }

    @Override
    protected void notifyStateSwitch(States newState) {
        stateObserver.stateChanged(newState);
    }
}
