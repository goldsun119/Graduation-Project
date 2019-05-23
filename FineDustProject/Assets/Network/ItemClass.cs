using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Game.Class
{
    public class ItemClass
    {
        private int id;
        private int type;
        private Vector3 position;
        private bool draw;
        
        
        public int get_id() { return this.id; }
        public Vector3 get_pos() { return this.position; }
        public bool get_draw() { return this.draw; }
        public int get_type() { return this.type; }
        
        public void set_id(int value) { this.id = value; }
        public void set_pos(Vector3 pos) { this.position = pos; }
        public void set_draw(bool value) { this.draw = value; }
        public void set_type(int i) { this.type = i; }

        public ItemClass(int id, int type, Vector3 pos)
        {
            this.id = id;
            this.type = type;
            this.position = pos;
            this.draw = true;
        }

        public ItemClass(int id, int type, Vector3 pos, bool d)
        {
            this.id = id;
            this.type = type;
            this.position = pos;
            this.draw = d;
        }
    }
}