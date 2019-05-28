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
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        player_st = player_tf.GetComponent<PlayerStatus>();
        Renderer = GetComponent<MeshRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Player"))
        {
            player_st.Enviroment_Level = Enviroment_Level;
            Renderer.material = Dome_Material_Fine;
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Player"))
        {
            player_st.Enviroment_Level = 1;
            Renderer.material = Dome_Material_Clear;

        }
    }
}
