using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class PlayerStatus : MonoBehaviour
{
    public int ID;

    public int hp;
    public int animator;
    public float vertical;
    public float horizontal;
    public Vector3 rotation;
    public Vector3 position;
    public bool connect;
    public bool draw;

    public float speedT = 5.0f;
    public float speedR = 2.0f;
    public float JumpP = 4.0f;

    public bool isMove = false, isRun = false, isJump = false, reJump = false;
    public bool isPick = false;
    public bool Die = false;

    // 상호작용
    public bool isItem = false;
    public bool isMon = false;

    //public int bagSize = 5;

    //public int itemCount = 0;

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        MoveStatus();
        RecvStatus();
    }

    void MoveStatus()
    {
        // 걷기
        if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D)) isMove = true;
        if (Input.GetKeyUp(KeyCode.W) || Input.GetKeyUp(KeyCode.S)||Input.GetKeyUp(KeyCode.A) || Input.GetKeyUp(KeyCode.D)) isMove = false;

        // 뛰기
        if (Input.GetKeyDown(KeyCode.LeftShift) && isMove)
        { isRun = true; speedT = 10.0f; }

        if (Input.GetKeyUp(KeyCode.LeftShift) || !isMove)
        { isRun = false; speedT = 5.0f; }

        // 점프
        if (Input.GetKeyDown(KeyCode.Space) && !reJump)
            isJump = true;

    }

    void OnCollisionEnter(Collision collision)
    {
        // 점프 해제
        if (collision.gameObject.CompareTag("Ground"))
        {
            reJump = false;
           // Debug.Log("땅임~");
        }

        // 아이템 충돌
        if (collision.gameObject.CompareTag("Item"))
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
    }

    void OnCollisionExit(Collision collision)
    {
        isPick = false;
        isItem = false;
        isMon = false;
    }

    void RecvStatus()
    {
        if(Game.Network.NetWork.client_data.ContainsKey(ID) && ID != Game.Network.NetWork.Client_id)
        {
            hp = Game.Network.NetWork.client_data[ID].get_hp();
            animator = Game.Network.NetWork.client_data[ID].get_animator();
            vertical = Game.Network.NetWork.client_data[ID].get_vertical();
            horizontal = Game.Network.NetWork.client_data[ID].get_horizontal();
            rotation = Game.Network.NetWork.client_data[ID].get_rot();
            position = Game.Network.NetWork.client_data[ID].get_pos();

            transform.position = position;
            transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);
        }
    }
}
