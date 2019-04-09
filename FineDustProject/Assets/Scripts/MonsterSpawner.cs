using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterSpawner : MonoBehaviour
{
    public GameObject monObj;

    public int monCnt = 1;
    public int monMaxCnt = 10;

    public Vector3 spawnPos;

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
            Debug.Log(monCnt);
        }
    }

    void Spawn()
    {
        spawnPos = RandomPosition();

        if ((spawnPos.x * spawnPos.x) + (spawnPos.z * spawnPos.z) <= (350 * 350))
        {
            monCnt--;
            Debug.Log("돔 안임");
            return;
        }

        spawnPos += Vector3.up * 0.6f;

        GameObject mon = Instantiate(monObj, spawnPos, Quaternion.identity);
    }

    Vector3 RandomPosition()
    {
        Vector3 monPosXZ = Random.insideUnitCircle * 900;
        Vector3 monPos = new Vector3(monPosXZ.x, 0f, monPosXZ.y);

        Debug.Log(monPos.x + ", " + monPos.y + ", " + monPos.z);

        return monPos;
    }
}
