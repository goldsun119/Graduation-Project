using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class ItemSpawner : MonoBehaviour
{
    public GameObject[] items;
    public Transform playerTransform;

    public int ItemCnt = 0;
    public int maxItemCnt = 5;
    public float maxDistance = 5f;

    public float timeBetSpawnMax = 7f;
    public float timeBetSpawnMin = 2f;
    private float timeBetSpawn;

    private float lastSpawnTime;


    // Start is called before the first frame update
    void Start()
    {
        timeBetSpawn = Random.Range(timeBetSpawnMin, timeBetSpawnMax);
        lastSpawnTime = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if(Time.time >= lastSpawnTime + timeBetSpawn && playerTransform != null )
        {
            if (ItemCnt < maxItemCnt)
            {
                lastSpawnTime = Time.time;
                timeBetSpawn = Random.Range(timeBetSpawnMin, timeBetSpawnMax);
                Spawn();
            }
        }
    }

    void Spawn()
    {
        Vector3 spawnPosition = GetRandomPointOnNavMesh(playerTransform.position, maxDistance);

        spawnPosition += Vector3.up * 0.5f;

        GameObject selectedItem = items[Random.Range(0, items.Length)];
        GameObject item = Instantiate(selectedItem, spawnPosition, Quaternion.identity);

        ItemCnt++;
    }

    Vector3 GetRandomPointOnNavMesh(Vector3 center, float distance)
    {
        Vector3 randomPos = Random.insideUnitSphere * distance + center;

        NavMeshHit hit;

        NavMesh.SamplePosition(randomPos, out hit, distance, NavMesh.AllAreas);

        return hit.position;
    }
}
