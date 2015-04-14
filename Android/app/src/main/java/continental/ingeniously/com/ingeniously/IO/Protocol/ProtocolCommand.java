package continental.ingeniously.com.ingeniously.IO.Protocol;

import continental.ingeniously.com.ingeniously.IO.CRC16;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public class ProtocolCommand {

    private final byte CRCgenerator = 0b01100100 ;
    private byte Header;
    private byte Right;
    private byte Forward;
    private short Time;
    private short CRC;

    public ProtocolCommand(){
        this((byte)0,(byte)0,(byte)0,(short)0);
    }

    public ProtocolCommand(byte right, byte forward, short time){
        this((byte)0,right,forward,time);
    }


    public boolean verifyCRC(){
      //  if(crc16(getBytes()) == 0){
        if(CRC16.crc16(getBytes()) == 0){
            return  true;
        }
        return false;

    }

    public ProtocolCommand(byte header, byte right, byte forward, short time){

        Header = header;
        Right = right;
        Forward = forward;
        Time = time;
        setCRC();
    }



    public byte[] getBytes(){
        byte comm[]= new byte[7];

        comm[0] = Header;
        comm[1] = Right;
        comm[2] = Forward;
        comm[3] = (byte) (Time>>8);
        comm[4] = (byte) Time;
        comm[5] = (byte) (CRC>>8);
        comm[6] = (byte) CRC;
        return comm;
    }

    private void setCRC(){

        ///
       //// CRC = (byte) 8;
        byte comm[]= new byte[5];

        comm[0] = Header;
        comm[1] = Right;
        comm[2] = Forward;
        comm[3] = (byte) (Time>>8);
        comm[4] = (byte) Time;
        CRC = CRC16.crc16(comm);

    }




    public short getCRC(){
        return CRC;
    }

    public void setRight(byte right) {
        Right = right;
    }

    public void setForward(byte forward) {
        Forward = forward;
    }

    public void setTime(short time) {
        Time = time;
    }

    public byte getRight() {
        return Right;
    }

    public byte getForward() {
        return Forward;
    }

    public byte getHeader() {
        return Header;
    }

    public void setHeader(byte header) {
        Header = header;
    }

    public short getTime() {
        return Time;
    }
}
