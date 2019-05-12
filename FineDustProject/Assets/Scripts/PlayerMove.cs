using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMove : MonoBehaviour
{
    PlayerStatus player;
    CameraMgr CMgr;
    Rigidbody rigid;

    Vector3 relativePos;

    GameObject playerObj;
    //public Camera CamT;

    void Start()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        player = playerObj.GetComponent<PlayerStatus>();
        rigid = GetComponent<Rigidbody>();
        CMgr = GameObject.Find("Players").GetComponent<CameraMgr>();
    }

    // Update is called once per frame
    void Update()
    {
        Move();
    }

    void Move()
    {

        if (player.Ani_State_Walk_Run >= PlayerStatus.ANI_TYPE.WALK)
        {
            if (CMgr.Camera_Num == 2)
            {

                if (Input.GetKey(KeyCode.W)) relativePos = Vector3.forward + Vector3.right - Vector3.zero;
                if (Input.GetKey(KeyCode.A)) relativePos = Vector3.forward + Vector3.left - Vector3.zero;
                if (Input.GetKey(KeyCode.S)) relativePos = Vector3.back + Vector3.left - Vector3.zero;
                if (Input.GetKey(KeyCode.D)) relativePos = Vector3.back + Vector3.right - Vector3.zero;

                if (Input.GetKey(KeyCode.W) && Input.GetKey(KeyCode.A)) relativePos = Vector3.forward - Vector3.zero;
                if (Input.GetKey(KeyCode.W) && Input.GetKey(KeyCode.D)) relativePos = Vector3.right - Vector3.zero;
                if (Input.GetKey(KeyCode.S) && Input.GetKey(KeyCode.A)) relativePos = Vector3.left - Vector3.zero;
                if (Input.GetKey(KeyCode.S) && Input.GetKey(KeyCode.D)) relativePos = Vector3.back - Vector3.zero;

                Quaternion rotation = Quaternion.LookRotation(relativePos, Vector3.up);
                transform.rotation = rotation;

                transform.Translate((Vector3.forward) * player.speedT * Time.deltaTime);

            }
            else
            {
                // 앞뒤 이동

                player.vertical = Input.GetAxis("Vertical");          // W, S 키
                transform.Translate(Vector3.forward * player.speedT * player.vertical * Time.deltaTime);

                // 회전
                player.horizontal = Input.GetAxis("Horizontal");        // Q, E 키 
                transform.Rotate(Vector3.up * player.speedR * player.horizontal);    // 회전

                if (Input.GetKey(KeyCode.A)) transform.Translate(Vector3.left * player.speedT * Time.deltaTime);
                if (Input.GetKey(KeyCode.D)) transform.Translate(Vector3.right * player.speedT * Time.deltaTime);

                Debug.Log(player.Direction_X);
            }
        }

        // 점프
        if (player.Ani_State_Jump == PlayerStatus.ANI_TYPE.IDEL)
        {
            rigid.AddForce(Vector3.up * player.JumpP, ForceMode.Impulse);
            player.Ani_State_Jump = PlayerStatus.ANI_TYPE.JUMP;
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


        player.position = playerObj.transform.position;
        player.rotation = new Vector3(playerObj.transform.eulerAngles.x, playerObj.transform.eulerAngles.y, playerObj.transform.eulerAngles.z);

        if (player.anitype != 0)
            Game.Network.NetWork.SendPlayerInfo(player.position, player.animator, player.Direction_X, player.Direction_Z, player.horizontal, player.vertical, player.rotation, player.name);

    }
}
