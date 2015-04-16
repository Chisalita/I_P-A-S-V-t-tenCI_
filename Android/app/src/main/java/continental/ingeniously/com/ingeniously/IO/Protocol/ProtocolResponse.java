package continental.ingeniously.com.ingeniously.IO.Protocol;

import continental.ingeniously.com.ingeniously.IO.CRC16;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public class ProtocolResponse {


    private byte Header = 0;
    private byte No_of_sensors = 0;
    private byte Info[];
    private short Time = 0;
    private short CRC = 0;


    public ProtocolResponse(){
        Header = 0;
        No_of_sensors = 0;
        Time = 0;
        CRC = 0;
    }

    public ProtocolResponse(ProtocolResponse copy){
        // call the functions to have all the checks done
        setHeader(copy.getHeader());
        setNo_of_sensors(copy.getNo_of_sensors());
        setInfo(copy.getInfo());
        setTime(copy.getTime());
        setCRC(copy.getCRC());
    }

    public void setHeader(byte header) {
        this.Header = header;
    }

    public byte getHeader() {
        return Header;
    }

    public byte getNo_of_sensors() {
        return No_of_sensors;
    }

    public void setNo_of_sensors(byte no_of_sensors) {
        No_of_sensors = no_of_sensors;
        if(Info == null){
            Info = new byte[no_of_sensors];
        }
    }

    public byte[] getInfo() {
        return Info;
    }

    public void setInfo(byte[] info) {
        if(Info == null){
            Info = new byte[info.length];
        }
        System.arraycopy( info, 0, Info, 0, info.length );
    }

    public void setInfo(byte info_i, int index) {
        Info[index] = info_i;
    }

    public short getTime() {
        return Time;
    }

    public void setTime(short time) {
        Time = time;
    }

    public void setTime_Low(byte low){
        this.Time |= low;
    }

    public void setTime_High(byte high){
        this.Time |= (short)(high<<8);
    }

    public short getCRC() {
        return CRC;
    }

    public byte[] getBytes(){
        byte body[];
        if(getNo_of_sensors()>0){
            body =  new byte[getNo_of_sensors()+6]; // make spase for the other things too
        }else {
            No_of_sensors = 0; //if it somehow is a negative number
            body = new byte[7]; // 1 byte for everything except time and CRC
            body[2] = 0; //preset the value for No_of_sensors in body
        }

        body[0] = Header;
        body[1] = No_of_sensors;

        int i=0;
        for(i =0; i<No_of_sensors; i++){
            body[2+i] = Info[i];
        }

        body[3+i] = (byte)(Time>>8); //continue with i
        body[4+i] = (byte)(Time & 0xff);
        body[5+i] = (byte) (CRC>>8);
        body[6+i] = (byte) (CRC & 0xff);

        return body;
    }

    private byte[] getBytes_no_CRC(){
        byte body[];
        if(getNo_of_sensors()>0){
         body =  new byte[getNo_of_sensors()+4]; // make spase for the other things too
        }else {
         No_of_sensors = 0; //if it somehow is a negative number
         body = new byte[5]; // 1 byte for everything except time
         body[2] = 0; //preset the value for No_of_sensors in body
        }

        body[0] = Header;
        body[1] = No_of_sensors;

        int i=0;
        for(i =0; i<No_of_sensors; i++){
            body[2+i] = Info[i];
        }

        body[3+i] = (byte)(Time>>8); //continue with i
        body[4+i] = (byte)(Time & 0xff);

        return body;
    }

    public void setCRC_Low(byte low){
        this.CRC |= low;
    }

    public void setCRC_High(byte high){
       this.CRC |= (short)(high<<8);
    }

    public void setCRC(byte[] crc) {
        if(crc.length  == 2) {
            System.arraycopy(crc, 0, CRC, 0, 2);
        }
    }

    public void setCRC(short crc) {
       this.CRC = crc;
    }

    public boolean verifyCRC(){
        if(CRC16.crc16(getBytes()) == 0){
            return  true;
        }
        return false;

    }

}
