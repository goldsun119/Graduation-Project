using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AreaMgr : MonoBehaviour
{
    public Transform player_tf;
    public PlayerStatus player_st;

    public bool is_Collide = false;
    public int Enviroment_Level = 1;

    public MeshRenderer Renderer;

    public Material Dome_Material_Clear;
    public Material Dome_Material_Fine;


    // Start is called before the first frame update
    void Start()
    {
        //string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        //GameObject playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        //// player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        //// player_st = player_tf.GetComponent<PlayerStatus>();
        //// PlayerStatus playerStatus = 
        //player_st = playerObj.GetComponent<PlayerStatus>();
        //Renderer = GetComponent<MeshRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
    }

    void OnTriggerEnter(Collider other)
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        GameObject playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        player_st = playerObj.GetComponent<PlayerStatus>();
        Renderer = GetComponent<MeshRenderer>();

        if (other.gameObject.CompareTag("Player") 
            && (other.gameObject.GetComponent<PlayerStatus>().ID == Game.Network.NetWork.Client_id))
        {
            player_st.Enviroment_Level = 0;

            if ((player_st.ID == Game.Network.NetWork.Client_id) && (player_st.Enviroment_Level == 0))
            {
                Renderer.material = Dome_Material_Fine;
                RenderSettings.fog = false;
            }

        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Player")
             && (other.gameObject.GetComponent<PlayerStatus>().ID == Game.Network.NetWork.Client_id))
        {
            player_st.Enviroment_Level = 1;
            if (player_st.ID == Game.Network.NetWork.Client_id && player_st.Enviroment_Level != 0)
            {
                Renderer.material = Dome_Material_Clear;
                RenderSettings.fog = true;

            }
        }
    }
}
