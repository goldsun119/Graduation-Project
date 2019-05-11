using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerStatus : MonoBehaviour
{
    float Player_Max_Walk_Speed = 2.7f;

    public int ID;

    public float speedT=0;
    public float speedR;
    public float JumpP;

    public int key_on_num = 0;
    public enum ANI_TYPE { IDEL, WALK, RUN, JUMP, REJUMP };
    public ANI_TYPE Ani_State_Walk_Run = (int)ANI_TYPE.IDEL;
    public ANI_TYPE Ani_State_Jump = (int)ANI_TYPE.IDEL;
    public bool isPick = false;

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
    }

    void MoveStatus()
    {
        // 걷기
        if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.D))
        {
            // idle 상태일때 키를 누르면 walk 상태로 전환
            if (Ani_State_Walk_Run == ANI_TYPE.IDEL) Ani_State_Walk_Run = ANI_TYPE.WALK;

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
            speedT = 0;
        }

        if ((Input.GetKeyDown(KeyCode.LeftShift)) && (Ani_State_Walk_Run <= ANI_TYPE.WALK))
        {
            Ani_State_Walk_Run = ANI_TYPE.RUN;
        }

        if ((Input.GetKeyUp(KeyCode.LeftShift)) && (Ani_State_Walk_Run == ANI_TYPE.RUN))
        {
            Ani_State_Walk_Run = ANI_TYPE.WALK;
        }

        // 점프
        if ((Input.GetKeyDown(KeyCode.Space)) && (Ani_State_Jump == ANI_TYPE.IDEL))
            Ani_State_Jump = ANI_TYPE.JUMP;

    }

    void OnCollisionEnter(Collision collision)
    {
        // 점프 해제
        if (collision.gameObject.CompareTag("Ground"))
        {
            Ani_State_Jump = (int)ANI_TYPE.IDEL;
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
}
