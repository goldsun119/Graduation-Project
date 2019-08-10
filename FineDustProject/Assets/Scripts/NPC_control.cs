using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPC_control : MonoBehaviour
{
    public CameraMgr CMgr;
    public bool is_collision;
    public PlayerStatus player;
    public bool NPC_ON = false;

    public GameObject UI_conversation;

    // Start is called before the first frame update
    void Start()
    {
        CMgr = GameObject.Find("Players").GetComponent<CameraMgr>();
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        player = GameObject.Find("Players").transform.Find(a).gameObject.GetComponent<PlayerStatus>();
        is_collision = false;
        UI_conversation.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (is_collision)
        {
            if (Input.GetKeyUp(KeyCode.F) && NPC_ON == true)
            {
                NPC_ON = false;
                CMgr.NPCCamOff(this.gameObject);
                player.is_move = true;
                UI_conversation.SetActive(false);
            }
            else if (Input.GetKeyUp(KeyCode.F) && NPC_ON == false)
            {
                NPC_ON = true;
                player.Ani_State_Walk_Run = PlayerStatus.ANI_TYPE.IDEL;
                UI_conversation.SetActive(true);
                //player.is_move = true;
            }
        }


        if (NPC_ON)
        {
            CMgr.NPCCamOn(this.gameObject);
            player.is_move = false;
        }

    }


    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player")
        {
            is_collision = true;
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.tag == "Player")
        {
            is_collision = false;
        }
    }

}
