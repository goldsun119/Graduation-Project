using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Game.Network;

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

        // 걷기
        player.vertical = Input.GetAxis("Vertical");          //앞, 뒤 키
        player.position = Vector3.forward * player.speedT * player.vertical * Time.deltaTime;
        transform.Translate(player.position);      //이동

        //Debug.Log("이동중");

        // 회전
        player.horizontal = Input.GetAxis("Horizontal");        //왼쪽, 오른쪽 키 
        player.rotation = Vector3.up * player.speedR * player.horizontal;
        transform.Rotate(player.rotation);    // 회전
        if(player.draw == true&&player.connect==true)
            NetWork.SendPlayerInfo(player.position, player.animator, player.horizontal, player.vertical, player.rotation, player.name);

        // float hor = Input.GetAxis("Horizontal");  
        //transform.Translate(Vector3.right * player.speedT * hor * Time.deltaTime);   

    }
}
