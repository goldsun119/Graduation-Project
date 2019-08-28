using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterSpawner : MonoBehaviour
{
    public GameObject monObj;

    //public int monCnt = 1;
    public int monID = -1;
    public int monMaxCnt = 100;

    public GameObject monsterPrefab;
    public GameObject monsterObj;
    public Vector2 Last_Spawn;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        //if (monCnt < monMaxCnt)
        //{
        //    Spawn();
        //    monCnt++;
        //    monID++;
        //    Debug.Log(monCnt);
        //}
        for (int i = 0; i < monMaxCnt; ++i)
        {
            if(Game.Network.NetWork.monster_data.ContainsKey(i) == false)
            {
                continue;
            }
            if (Game.Network.NetWork.monster_data[i].get_hp()<=0 || Game.Network.NetWork.monster_data[i].get_draw() == false)
            {
                continue;
            }
            string a = "Monster(" + i.ToString() + ")";
            //GameObject playerObj = transform.GetChild(id-1).gameObject;
            Transform tmp = transform.Find(a);
            if (tmp == null)
            {
                GameObject monster = Instantiate(monsterPrefab,
                                                    Game.Network.NetWork.monster_data[i].get_pos(),
                                                    Quaternion.identity);
                monster.name = a;
                monster.transform.parent = transform;
                monsterObj = transform.Find(a).gameObject;
            }
            else
            {
                monsterObj = transform.Find(a).gameObject;
            }
            MonsterMgr monsterStatus = monsterObj.GetComponent<MonsterMgr>();
            if (Game.Network.NetWork.monster_data.ContainsKey(i))
            {
                //    playerStatus.rotation = Game.Network.NetWork.client_data[i].get_rot();
                monsterStatus.ID = i;
                monsterStatus.position = Game.Network.NetWork.monster_data[i].get_pos();
                monsterStatus.rotation = Game.Network.NetWork.monster_data[i].get_rot();
                monsterStatus.dirX = Game.Network.NetWork.monster_data[i].get_dirX();
                monsterStatus.dirZ = Game.Network.NetWork.monster_data[i].get_dirZ();
                monsterStatus.chase_id = Game.Network.NetWork.monster_data[i].get_chase_id();
                monsterStatus.calculate_id = Game.Network.NetWork.monster_data[i].get_calculate_id();
                monsterStatus.animator = Game.Network.NetWork.monster_data[i].get_animator();
                
                //monsterStatus.draw = true;
            }
            else
            {
                //monsterStatus.draw = false;
            }
            //if (monsterStatus.draw)
            //{
                monsterObj.SetActive(true);

            //}
            //else
            //{
            //    monsterObj.SetActive(false);
            //}
            //        last_id = Game.Network.NetWork.new_player_id;
        }
    }

    //void Spawn()
    //{
    //    Vector3 spawnPos;

    //    spawnPos = RandomPosition();

    //    if ((spawnPos.x * spawnPos.x) + (spawnPos.z * spawnPos.z) <= (350 * 350))
    //    {
    //        monCnt--;
    //        Debug.Log("돔 안임");
    //        return;
    //    }

    //    //spawnPos += Vector3.up * 0.6f;

    //    GameObject mon = Instantiate(monObj, spawnPos, Quaternion.identity);
    //    Last_Spawn = spawnPos;
    //    Debug.Log("몬스터 생성" + monCnt + " " + spawnPos.x + ", " + spawnPos.y + ", " + spawnPos.z);
    //}

    //Vector3 RandomPosition()
    //{
    //    Vector3 monPosXZ = Random.insideUnitCircle * 900;
    //    Vector3 monPos = new Vector3(monPosXZ.x, 70f, monPosXZ.y);

    //    return monPos;
    //}
}
