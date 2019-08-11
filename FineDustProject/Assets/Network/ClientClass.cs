using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Game.Class
{
    public class ClientClass
    {
        private int id;
        private int hp;
        private int animator;
        private float dirX;
        private float dirZ;
        private float vertical;
        private float horizontal;
        private Vector3 rotation;
        private Vector3 position;
        private bool connect;
        private bool draw;
        private string name;
        private int character;
        private int[] item = new int[4];
        private int[] item_count = new int[4];

        //private bool prefab;    // 클라이언트 프리팹이 만들어졌는지 확인
        //public GameObject player;   // 프리팹을 위한 게임 오브젝트
        //private bool activePlayer;
        //public OtherPlayerCtrl script;  // 프리팹 오브젝트 안의 함수를 호출하기 위한 스크립트

        public float get_vertical() { return this.vertical; }
        public float get_horizontal() { return this.horizontal; }
        public int get_id() { return this.id; }
        public int get_hp() { return this.hp; }
        public int get_animator() { return this.animator; }
        public float get_dirX() { return this.dirX; }
        public float get_dirZ() { return this.dirZ; }
        public Vector3 get_pos() { return this.position; }
        public Vector3 get_rot() { return this.rotation; }
        public bool get_connect() { return this.connect; }
        public bool get_draw() { return this.draw; }
        public string get_name() { return this.name; }
        public int get_char() { return this.character; }
        public int get_item(int index) { return this.item[index]; }
        public int get_item_count(int index) { return this.item_count[index]; }
        //public bool get_prefab() { return this.prefab; }
        //public bool get_activePlayer() { return this.activePlayer; }

        public void set_vertical(float value) { this.vertical = value; }
        public void set_horizontal(float value) { this.horizontal = value; }
        public void set_id(int value) { this.id = value; }
        public void set_hp(int value) { this.hp = value; }
        public void set_animator(int value) { this.animator = value; }
        public void set_dirX(float value) { this.dirX = value; }
        public void set_dirZ(float value) { this.dirZ = value; }
        public void set_pos(Vector3 pos) { this.position = pos; }
        public void set_rot(Vector3 rot) { this.rotation = rot; }
        public void set_connect(bool value) { this.connect = value; }
        public void set_draw(bool value) { this.draw = value; }
        public void set_name(string name) { this.name = name; }
        public void set_char(int value) { this.character = value; }
        public void set_item(int value, int index) { this.item[index] = value; }    
        public void set_item_count(int value, int index) { this.item_count[index] = value; }
        //public void set_prefab(bool value) { this.prefab = value; }
        //public void set_activePlayer(bool value) { this.activePlayer = value; }

        public ClientClass(int id, int hp, string name, Vector3 pos, Vector3 rot)
        {
            this.id = id;
            this.hp = hp;
            this.name = name;
            this.position = pos;
            this.rotation = rot;
            this.animator = 0;
            this.connect = true;
            this.vertical = 0.0f;
            this.horizontal = 0.0f;
            //this.prefab = false;
            //this.player = null;
            ////this.script = null;
            //this.activePlayer = true;
        }
        public ClientClass(int id, int hp, int ani, float h, float v, string name, Vector3 pos, Vector3 rot)
        {
            this.id = id;
            this.hp = hp;
            this.animator = ani;
            this.horizontal = h;
            this.vertical = v;
            this.name = name;
            this.position = pos;
            this.rotation = rot;
            this.connect = true;
            //this.prefab = false;
            //this.player = null;
            ////this.script = null;
            //this.activePlayer = true;
        }

        public ClientClass(int id, int hp, int ani, float x, float z, float h, float v, string name, Vector3 pos, Vector3 rot)
        {
            this.id = id;
            this.hp = hp;
            this.animator = ani;
            this.dirX = x;
            this.dirZ = z;
            this.horizontal = h;
            this.vertical = v;
            this.name = name;
            this.position = pos;
            this.rotation = rot;
            this.connect = true;
            //this.prefab = false;
            //this.player = null;
            ////this.script = null;
            //this.activePlayer = true;
        }


        public ClientClass(int id, string name, Vector3 pos, int hp, int i1, int i2, int i3, int i4, int ch, int ic1, int ic2, int ic3,int ic4)
        {
            this.id = id;
            this.name = name;
            this.position = pos;
            this.hp = hp;
            this.item[0] = i1;
            this.item[1] = i2;
            this.item[2] = i3;
            this.item[3] = i4;
            this.character = ch;
            this.connect = true;
            this.item_count[0] = ic1;
            this.item_count[1] = ic2;
            this.item_count[2] = ic3;
            this.item_count[3] = ic4;
            //this.prefab = false;
            //this.player = null;
            ////this.script = null;
            //this.activePlayer = true;
        }
    }
}