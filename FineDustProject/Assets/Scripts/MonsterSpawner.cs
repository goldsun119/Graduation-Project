using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterSpawner : MonoBehaviour
{
    public GameObject monObj;

    public int monCnt = 1;
    public int monID = 1;
    public int monMaxCnt = 10;

    public Vector2 Last_Spawn;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (monCnt < monMaxCnt)
        {
            Spawn();
            monCnt++;
            monID++;
            Debug.Log(monCnt);
        }
    }

    void Spawn()
    {
        Vector3 spawnPos;

        spawnPos = RandomPosition();

        if ((spawnPos.x * spawnPos.x) + (spawnPos.z * spawnPos.z) <= (350 * 350))
        {
            monCnt--;
            Debug.Log("돔 안임");
            return;
        }

        //spawnPos += Vector3.up * 0.6f;

        GameObject mon = Instantiate(monObj, spawnPos, Quaternion.identity);
        Last_Spawn = spawnPos;
        Debug.Log("몬스터 생성" + monCnt + " " + spawnPos.x + ", " + spawnPos.y + ", " + spawnPos.z);
    }

    Vector3 RandomPosition()
    {
        Vector3 monPosXZ = Random.insideUnitCircle * 900;
        Vector3 monPos = new Vector3(monPosXZ.x, 70f, monPosXZ.y);

        return monPos;
    }
}
