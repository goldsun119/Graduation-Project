using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Alpha_Change : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnTriggerEnter(Collider other)
    {
        Debug.Log(other.name + "가려져라" );
        other.GetComponent<MeshRenderer>().material.color = new Color(0f, 0f, 0f, 0.3f);
    }
}
