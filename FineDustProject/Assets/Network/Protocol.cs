using System.Collections;
using System.Collections.Generic;

namespace Game.Protocol
{
    public class Protocol
    {
        public int SC_ID;
        public int SC_PUT_PLAYER;
        public int SC_REMOVE_PLAYER;
        public int SC_ALL_PLAYER_DATA;
        public int SC_PUT_MONSTER;
        public int SC_REMOVE_MONSTER;

        public int CS_INFO;
        public int CS_GET_ITEM;
        public int CS_MONSTER_HP;

        public Protocol()
        {
            SC_ID = 1;
            SC_PUT_PLAYER = 2;
            SC_REMOVE_PLAYER = 3;
            SC_ALL_PLAYER_DATA = 4;
            SC_PUT_MONSTER = 5;
            SC_REMOVE_MONSTER = 6;

            CS_INFO = 1;
            CS_GET_ITEM = 2;
            CS_MONSTER_HP = 3;
        }
    }
}