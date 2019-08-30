using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class players : MonoBehaviour
{
    int login = 0;
    public GameObject playerPrefab;
    private GameObject playerObj;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //if(last_id != Game.Network.NetWork.new_player_id)
        //{
            for (int i = 1; i <= Game.Protocol.Protocol.MAX_USER; ++i)
            {
                string a = "Player(" + i.ToString() + ")";
            //GameObject playerObj = transform.GetChild(id-1).gameObject;
            Transform tmp = transform.Find(a);
            if (tmp == null)
            {
                GameObject player = Instantiate(playerPrefab,
                                                    Game.Network.NetWork.client_data[i].get_pos(),
                                                    Quaternion.identity);
                player.name = a;
                player.transform.parent = transform;
                playerObj = transform.Find(a).gameObject;
            }
            else
            {
                playerObj = transform.Find(a).gameObject;
            }
                PlayerStatus playerStatus = playerObj.GetComponent<PlayerStatus>();
                if (Game.Network.NetWork.client_data.ContainsKey(i))
                {
                //    playerStatus.rotation = Game.Network.NetWork.client_data[i].get_rot();
                playerStatus.draw = Game.Network.NetWork.client_data[i].get_draw();
                playerStatus.ID = i;
                }
            if (playerStatus.draw)
            {
                playerObj.SetActive(true);
                if (login < 1)
                {
                    //playerStatus.position = Game.Network.NetWork.client_data[i].get_pos();
                    login++;
                }

            }
            else
            {
                playerObj.SetActive(false);
            }
            //        last_id = Game.Network.NetWork.new_player_id;
            }
        //}
    }

    public void spawn(int id)
    {

    }
}