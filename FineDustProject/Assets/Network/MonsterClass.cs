using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Game.Class
{
    public class MonsterClass
    {
        private int id;
        private int chase_id;
        private int hp;
        private int animator;
        private float dirX;
        private float dirZ;
        private Vector3 rotation;
        private Vector3 position;
        private bool draw;
        
        public int get_id() { return this.id; }
        public int get_chase_id() { return this.chase_id; }
        public int get_hp() { return this.hp; }
        public int get_animator() { return this.animator; }
        public float get_dirX() { return this.dirX; }
        public float get_dirZ() { return this.dirZ; }
        public Vector3 get_pos() { return this.position; }
        public Vector3 get_rot() { return this.rotation; }
        public bool get_draw() { return this.draw; }
        
        public void set_id(int value) { this.id = value; }
        public void set_chase_id(int value) { this.chase_id = value; }
        public void set_hp(int value) { this.hp = value; }
        public void set_animator(int value) { this.animator = value; }
        public void set_dirX(float value) { this.dirX = value; }
        public void set_dirZ(float value) { this.dirZ = value; }
        public void set_pos(Vector3 pos) { this.position = pos; }
        public void set_rot(Vector3 rot) { this.rotation = rot; }
        public void set_draw(bool value) { this.draw = value; }
        
        public MonsterClass(int id, int hp, int ani, float x, float z, Vector3 pos, Vector3 rot)
        {
            this.id = id;
            this.hp = hp;
            this.animator = ani;
            this.dirX = x;
            this.dirZ = z;
            this.position = pos;
            this.rotation = rot;
        }
    }
}