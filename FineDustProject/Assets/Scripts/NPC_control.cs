using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPC_control : MonoBehaviour
{
    private CameraMgr CMgr;
    private bool is_collision;
    private PlayerStatus player;
    public bool NPC_ON = false;
    public Animator animator;
    public int ani_type;
    public IEnumerator coroutine;

    public GameObject UI_conversation;

    // Start is called before the first frame update
    void Start()
    {
        CMgr = GameObject.Find("Players").GetComponent<CameraMgr>();
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        player = GameObject.Find("Players").transform.Find(a).gameObject.GetComponent<PlayerStatus>();
        is_collision = false;
        UI_conversation.SetActive(false);
        ani_type = 0;
        
    }

    void Awake()
    {
        CMgr = GameObject.Find("Players").GetComponent<CameraMgr>();
        //string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        //player = GameObject.Find("Players").transform.Find(a).gameObject.GetComponent<PlayerStatus>();
        is_collision = false;
        UI_conversation.SetActive(false);
        ani_type = 0;
        animator = GetComponent<Animator>();
        coroutine = HPControl();
        StartCoroutine(coroutine);

    }

    IEnumerator HPControl()
    {
        while (true)
        {
            ani_type = (ani_type + 1) % 3;
            //if (hp > 0)
            //    hp -= Enviroment_Level;
            //else
            //    StopCoroutine(coroutine);
            
            yield return new WaitForSeconds(5);//WaitForSeconds객체를 생성해서 반환
                                               //StartCoroutine(HPControl());
        }
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

        animator.SetInteger("Ani_type", ani_type);

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

    void MoveAni()
    {
        if (player.ID == Game.Network.NetWork.Client_id)
        {

            if (player.Ani_State == PlayerStatus.ANI_TYPE.IDEL) player.anitype = (int)PlayerStatus.ANI_TYPE.IDEL;
            else if (player.Ani_State == PlayerStatus.ANI_TYPE.WALK) player.anitype = (int)PlayerStatus.ANI_TYPE.WALK;
            else if (player.Ani_State == PlayerStatus.ANI_TYPE.RUN) player.anitype = (int)PlayerStatus.ANI_TYPE.RUN;
            else if (player.Ani_State == PlayerStatus.ANI_TYPE.DIE) player.anitype = (int)PlayerStatus.ANI_TYPE.DIE;
            else animator.SetInteger("Ani_type", 0);

            //if (CMgr.Camera_Num == 2 && player.anitype != 0)
            //    player.anitype += 2;
        }

        animator.SetInteger("Ani_type", ani_type);
    }
}
