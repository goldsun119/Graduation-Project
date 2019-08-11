using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerStatus : MonoBehaviour
{
    float Player_Max_Walk_Speed = 3f;

    public int ID;

    //
    public int hp;
    //public int animator;
    public float vertical;
    public float horizontal;
    public Vector3 rotation;
    public Vector3 position;
    public bool connect;
    public bool draw;
    public int anitype = 0;
    public float Direction_X = 0;
    public float Direction_Z = 0;
    public int Enviroment_Level = 0;
    //

    public float speedT = 0;
    public float speedR;
    public float JumpP;

    public int key_on_num = 0;
    public enum ANI_TYPE { IDEL, WALK, RUN, JUMP };
    
    public ANI_TYPE Ani_State_Walk_Run = ANI_TYPE.IDEL;
    public ANI_TYPE Ani_State_Jump = ANI_TYPE.IDEL;

    public bool isPick = false;

    // 상호작용
    public bool isItem = false;
    public bool isMon = false;
    public bool is_move = true;
    public bool Inventory_on = false;

    public IEnumerator coroutine;

    CameraMgr CMgr;
    //public int bagSize = 5;

    //public int itemCount = 0;
    public MonsterSpawner MonSpawner;

    public void SetLayersRecursively(Transform trans, string name)
    {
        trans.gameObject.layer = LayerMask.NameToLayer(name);
        foreach (Transform child in trans)
            SetLayersRecursively(child, name);
    }

    void Start()
    {
        CMgr = GetComponent<CameraMgr>();
        hp = 100;
        coroutine = HPControl();
        StartCoroutine(coroutine);
        if (ID == Game.Network.NetWork.Client_id)
        {
            SetLayersRecursively(this.gameObject.transform, "myPlayer");
        }



    }

        // Update is called once per frame
        void Update()
    {
        if (ID == Game.Network.NetWork.Client_id)
        {
            if (is_move)
            {
                MoveStatus();
            }

            


        }
        RecvStatus();
    }

    IEnumerator HPControl()
    {
        while (true)
        {
            if (hp > 0)
                hp -= Enviroment_Level;
            else
                StopCoroutine(coroutine);

            Debug.Log("hp");
            yield return new WaitForSeconds(2);//WaitForSeconds객체를 생성해서 반환
                                               //StartCoroutine(HPControl());
        }
    }

    void MoveStatus()
    {
        
        // 방향에 따라 Direction 변화
        if (Input.GetKeyDown(KeyCode.W)) Direction_Z = 1;
        if (Input.GetKeyDown(KeyCode.S)) Direction_Z = -1;
        if (Input.GetKeyDown(KeyCode.A)) Direction_X = -1;
        if (Input.GetKeyDown(KeyCode.D)) Direction_X = 1;
        if (Input.GetKeyDown(KeyCode.Q))
        {
            Direction_Z = 1;
            Direction_X = -0.1f;
        }
        if (Input.GetKeyDown(KeyCode.E))
        {
            Direction_Z = 1;
            Direction_X = 0.1f;
        }

        if (Input.GetKeyUp(KeyCode.W) || Input.GetKeyUp(KeyCode.S)) Direction_Z = 0;
        if (Input.GetKeyUp(KeyCode.D) || Input.GetKeyUp(KeyCode.A)) Direction_X = 0;
        //if (Input.GetKeyUp(KeyCode.Q) || Input.GetKeyUp(KeyCode.E)) Direction_X = 0;

        // 걷기
        if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.Q) || Input.GetKey(KeyCode.E))
        {
            // idle 상태일때 키를 누르면 walk 상태로 전환
            if (Ani_State_Walk_Run == ANI_TYPE.IDEL) Ani_State_Walk_Run = ANI_TYPE.WALK;


            if (Input.GetKey(KeyCode.W))
            {
                Direction_Z = 1;
                if (Input.GetKey(KeyCode.A)) Direction_X = -1;
                if (Input.GetKey(KeyCode.D)) Direction_X = 1;
            }
            if (Input.GetKey(KeyCode.S))
            {
                Direction_Z = -1;
                if (Input.GetKey(KeyCode.A)) Direction_X = -1;
                if (Input.GetKey(KeyCode.D)) Direction_X = 1;

            }
            if (Input.GetKey(KeyCode.A))
            {
                Direction_X = -1;
                if (Input.GetKey(KeyCode.W)) Direction_Z = 1;
                if (Input.GetKey(KeyCode.S)) Direction_Z = -1;
            }
            if (Input.GetKey(KeyCode.D))
            {
                Direction_X = 1;
                if (Input.GetKey(KeyCode.W)) Direction_Z = 1;
                if (Input.GetKey(KeyCode.S)) Direction_Z = -1;
            }
            
            // 반대 방향키를 같이 누르고 있으면 제자리 서기
            if ((Input.GetKey(KeyCode.W) && Input.GetKey(KeyCode.S)) || (Input.GetKey(KeyCode.A) && Input.GetKey(KeyCode.D)) || (Input.GetKey(KeyCode.Q) && Input.GetKey(KeyCode.E)))
            {
                Ani_State_Walk_Run = ANI_TYPE.IDEL;
                Direction_X = 0;
                Direction_Z = 0;
            }

            // walk인지 run인지에 따라 속도 변화
            if ((Ani_State_Walk_Run == ANI_TYPE.WALK) && speedT < Player_Max_Walk_Speed)
                speedT += 0.2f;
            else if (Ani_State_Walk_Run == ANI_TYPE.RUN)
                speedT = 8f;
            else speedT = Player_Max_Walk_Speed;
        }
        else
        {
            Ani_State_Walk_Run = ANI_TYPE.IDEL;
            Direction_X = 0;
            Direction_Z = 0;
            speedT = 0;
        }

        if ((Input.GetKeyDown(KeyCode.LeftShift)) && (Ani_State_Walk_Run <= ANI_TYPE.WALK))
        {
            Ani_State_Walk_Run = ANI_TYPE.RUN;
        }
        if ((Input.GetKey(KeyCode.LeftShift)) && (Ani_State_Walk_Run <= ANI_TYPE.WALK))
        {
            Ani_State_Walk_Run = ANI_TYPE.RUN;
        }
        if ((Input.GetKeyUp(KeyCode.LeftShift)) && (Ani_State_Walk_Run == ANI_TYPE.RUN))
        {
            Ani_State_Walk_Run = ANI_TYPE.WALK;
        }

        // Shift 키를 누르고 있는데 방향키는 안누른 상태 --> 달리기 멈추기
        if (Input.GetKey(KeyCode.LeftShift) && Direction_X == 0 && Direction_Z == 0)
            Ani_State_Walk_Run = ANI_TYPE.IDEL;

        // 점프
        if ((Input.GetKeyDown(KeyCode.Space)) && (Ani_State_Jump == ANI_TYPE.IDEL))
            Ani_State_Jump = ANI_TYPE.JUMP;

        CheatKey();
    }

    void OnCollisionEnter(Collision collision)
    {
        // 점프 해제
        if (collision.gameObject.CompareTag("Ground"))
        {
            Ani_State_Jump = ANI_TYPE.IDEL;
        }

        

        // 문 충돌
        if (collision.gameObject.CompareTag("Door"))
        {
            isItem = true;

            if (Input.GetKey(KeyCode.F))
            {
                isPick = true;
                isItem = false;
            }
        }

        // 몬스터 충돌
        if (collision.gameObject.CompareTag("Monster"))
        {
            isMon = true;
        }

        // 지붕등 충돌
        if (collision.gameObject.CompareTag("wall"))
        {
            SetLayersRecursively(collision.gameObject.transform, "wall");
        }
    }

    void OnCollisionExit(Collision collision)
    {
        isPick = false;
        isMon = false;

        if (collision.gameObject.CompareTag("wall"))
        {
            SetLayersRecursively(collision.gameObject.transform, "Default");
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        // 아이템 충돌
        if (other.gameObject.CompareTag("Item"))
        {
            isItem = true;

            if (Input.GetKey(KeyCode.F))
            {
                isPick = true;
                isItem = false;
            }
        }

        // 지붕등 충돌
        if (other.gameObject.CompareTag("wall"))
        {
            SetLayersRecursively(other.gameObject.transform, "wall");
        }
    }

    private void OnTriggerExit(Collider other)
    {
        isItem = false;
        // 지붕등 충돌
        if (other.gameObject.CompareTag("wall"))
        {
            SetLayersRecursively(other.gameObject.transform, "Default");
        }
    }

    void CheatKey()
    {
        if (Input.GetKeyDown(KeyCode.P)) transform.position = new Vector3(0, 30, 360);
        if (Input.GetKeyDown(KeyCode.O)) transform.position = new Vector3(0, 30, 50);
        if (Input.GetKeyDown(KeyCode.L)) transform.position = new Vector3(0, 30, -5);
        //if (Input.GetKeyDown(KeyCode.L)) transform.position = ;
        //if (Input.GetKeyDown(KeyCode.K)) transform.position = MonSpawner.Last_Spawn;

    }

    void RecvStatus()
    {
        if (Game.Network.NetWork.client_data.ContainsKey(ID) && ID != Game.Network.NetWork.Client_id)
        {
            hp = Game.Network.NetWork.client_data[ID].get_hp();
            anitype = Game.Network.NetWork.client_data[ID].get_animator();
            vertical = Game.Network.NetWork.client_data[ID].get_vertical();
            horizontal = Game.Network.NetWork.client_data[ID].get_horizontal();
            rotation = Game.Network.NetWork.client_data[ID].get_rot();
            position = Game.Network.NetWork.client_data[ID].get_pos();
            Direction_X = Game.Network.NetWork.client_data[ID].get_dirX();
            Direction_Z = Game.Network.NetWork.client_data[ID].get_dirZ();
            transform.position = position;
            transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);
            draw = Game.Network.NetWork.client_data[ID].get_draw();
        }
    }
}
