using System.Collections;
using System.Collections.Generic;

namespace Game.Protocol
{
    public class Protocol
    {
        public static int SC_ID = 1;
        public static int SC_PUT_PLAYER =				2;
        public static int SC_REMOVE_PLAYER =		3;
        public static int SC_PLAYER_STATUS	 =		4;
        public static int SC_ALL_PLAYER_DATA =	5;
        public static int SC_PUT_MONSTER =			6;
        public static int SC_REMOVE_MONSTER =	7;
        public static int SC_PUT_ITEM =				    8;
        public static int SC_REMOVE_ITEM =			9;
        public static int SC_PUT_IN_BAG			 =	    10;
        public static int SC_INIT_DATA			 =	    11;
        public static int SC_LOGIN_SUCCESS =       12;
        public static int SC_LOGIN_FAIL =              13;
        public static int SC_SIGNUP =                     14;

        public static int CS_INFO =                         1;
        public static int CS_GET_ITEM =                 2;
        public static int CS_MONSTER_STATUS =   3;
        public static int CS_LOGIN                       = 4;

        public static int MAX_USER = 4;   // 0번째는 버림

        public Protocol()
        {

        }
    }
}