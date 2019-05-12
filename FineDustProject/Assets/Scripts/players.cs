using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class players : MonoBehaviour
{
    int last_id = 0;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(last_id != Game.Network.NetWork.new_player_id)
        {
            for (int i = 1; i <= Game.Network.NetWork.new_player_id; ++i)
            {
                string a = "Player(" + i.ToString() + ")";
                //GameObject playerObj = transform.GetChild(id-1).gameObject;
                GameObject playerObj = transform.Find(a).gameObject;
                PlayerStatus playerStatus = playerObj.GetComponent<PlayerStatus>();
                if (Game.Network.NetWork.client_data.ContainsKey(i))
                {
                    playerStatus.position = Game.Network.NetWork.client_data[i].get_pos();
                    playerStatus.rotation = Game.Network.NetWork.client_data[i].get_rot();
                }
                playerObj.SetActive(true);
                last_id = Game.Network.NetWork.new_player_id;
            }
        }
    }

    public void spawn(int id)
    {

    }
}
