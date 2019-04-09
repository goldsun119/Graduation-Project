using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerStatus : MonoBehaviour
{
    public float speedT = 5.0f;
    public float speedR = 2.0f;
    public float JumpP = 4.0f;

    public bool isMove = false, isRun = false, isJump = false, reJump = false;
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
        if (Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.S)) isMove = true;
        if (Input.GetKeyUp(KeyCode.W) || Input.GetKeyUp(KeyCode.S)) isMove = false;

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
            Debug.Log("땅임~");
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
