using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMove : MonoBehaviour
{
    PlayerStatus player;

    Rigidbody rigid;

    void Start()
    {
        player = GetComponent<PlayerStatus>();
        rigid = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        Move();
    }

    void Move()
    {
        // 걷기
        if (player.isMove)
        {
            float ver = Input.GetAxis("Vertical");          //앞, 뒤 키

            transform.Translate(Vector3.forward * player.speedT * ver * Time.deltaTime);      //이동
            Debug.Log("이동중");
        }
        // 회전
        float hor = Input.GetAxis("Horizontal");        //왼쪽, 오른쪽 키 
        transform.Rotate(Vector3.up * player.speedR * hor);    // 회전

        // 점프
        if (player.isJump)
        {
            rigid.AddForce(Vector3.up * player.JumpP, ForceMode.Impulse);
            player.isJump = false;
        }

        // 충돌 - 아이템
        if (player.isItem)
        {
            Debug.Log("item collision");
        }
        if (!player.isItem)
        {

        }

        // 충돌 - 몬스터
        if (player.isMon)
        {
            Debug.Log("monster collision");
        }
        if (!player.isMon)
        {

        }

    }
}
