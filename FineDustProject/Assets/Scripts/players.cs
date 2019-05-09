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
            string a = "Player(" + Game.Network.NetWork.new_player_id.ToString() + ")";
            //GameObject playerObj = transform.GetChild(id-1).gameObject;
            GameObject playerObj = transform.Find(a).gameObject;
            PlayerStatus playerStatus = playerObj.GetComponent<PlayerStatus>();

            if (Game.Network.NetWork.new_player_id == playerStatus.ID)
                playerObj.SetActive(true);
            last_id = Game.Network.NetWork.new_player_id;
        }
    }

    public void spawn(int id)
    {

    }
}
