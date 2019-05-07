using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMove : MonoBehaviour
{
    PlayerStatus player;

    Rigidbody rigid;

    //public Camera CamT;

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
        // 앞뒤 이동
        float ver_move = Input.GetAxis("Vertical");          // W, S 키
        transform.Translate(Vector3.forward * player.speedT * ver_move * Time.deltaTime);
        Debug.Log("이동중");

        // 회전
        //if (CamT.enabled == true)
        //if(transform.Find("Cam3").gameObject)
        {
            float hor_rotate = Input.GetAxis("Horizontal");        // Q, E 키 
            transform.Rotate(Vector3.up * player.speedR * hor_rotate);    // 회전
        }

        // 좌우 이동
        if (Input.GetKey(KeyCode.A))
            transform.Translate(Vector3.left * player.speedT * Time.deltaTime);
        if (Input.GetKey(KeyCode.D))
            transform.Translate(Vector3.right * player.speedT * Time.deltaTime);      //이동
        
        // 점프
        if (player.isJump)
        {
            rigid.AddForce(Vector3.up * player.JumpP, ForceMode.Impulse);
            player.isJump = false;
            player.reJump = true;
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
