using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPC_control : MonoBehaviour
{
    private CameraMgr CMgr;
    private bool is_collision;
    private PlayerStatus player;
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

    void Awake()
    {
        CMgr = GameObject.Find("Players").GetComponent<CameraMgr>();
        //string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        //player = GameObject.Find("Players").transform.Find(a).gameObject.GetComponent<PlayerStatus>();
        is_collision = false;
        UI_conversation.SetActive(false);

    }

        // Update is called once per frame
        void Update()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        player = GameObject.Find("Players").transform.Find(a).gameObject.GetComponent<PlayerStatus>();
        

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
                player.Ani_State = PlayerStatus.ANI_TYPE.IDEL;
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
            NPC_ON = false;
            CMgr.NPCCamOff(this.gameObject);
            player.is_move = true;
            UI_conversation.SetActive(false);
        }
    }

}
