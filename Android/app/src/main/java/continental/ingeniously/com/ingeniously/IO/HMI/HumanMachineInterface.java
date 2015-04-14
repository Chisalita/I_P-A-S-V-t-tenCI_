package continental.ingeniously.com.ingeniously.IO.HMI;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public interface HumanMachineInterface {
    HMI_Command getLastCommand();
    HMI_Info getLastInfoShown();
    void sendInfo(HMI_Info info);

}
