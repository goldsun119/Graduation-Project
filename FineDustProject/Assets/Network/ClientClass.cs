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
        private float vertical;
        private float horizontal;
        private Vector3 rotation;
        private Vector3 position;
        private bool connect;
        private bool draw;
        private string name;

        private bool prefab;    // 클라이언트 프리팹이 만들어졌는지 확인
        public GameObject player;   // 프리팹을 위한 게임 오브젝트
        private bool activePlayer;
        //public OtherPlayerCtrl script;  // 프리팹 오브젝트 안의 함수를 호출하기 위한 스크립트

        public float get_vertical() { return this.vertical; }
        public float get_horizontal() { return this.horizontal; }
        public int get_id() { return this.id; }
        public int get_hp() { return this.hp; }
        public int get_animator() { return this.animator; }
        public Vector3 get_pos() { return this.position; }
        public Vector3 get_rot() { return this.rotation; }
        public bool get_connect() { return this.connect; }
        public bool get_draw() { return this.draw; }
        public string get_name() { return this.name; }
        public bool get_prefab() { return this.prefab; }
        public bool get_activePlayer() { return this.activePlayer; }

        public void set_vertical(float value) { this.vertical = value; }
        public void set_horizontal(float value) { this.horizontal = value; }
        public void set_id(int value) { this.id = value; }
        public void set_hp(int value) { this.hp = value; }
        public void set_animator(int value) { this.animator = value; }
        public void set_pos(Vector3 pos) { this.position = pos; }
        public void set_rot(Vector3 rot) { this.rotation = rot; }
        public void set_connect(bool value) { this.connect = value; }
        public void set_draw(bool value) { this.draw = value; }
        public void set_name(string name) { this.name = name; }
        public void set_prefab(bool value) { this.prefab = value; }
        public void set_activePlayer(bool value) { this.activePlayer = value; }

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
            this.prefab = false;
            this.player = null;
            //this.script = null;
            this.activePlayer = true;
        }
    }
}