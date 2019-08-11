using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Alpha_Change : MonoBehaviour
{

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Untagged"))
        {
            string nameShader = "Transparent/VertexLit";

            MeshRenderer renderer = other.gameObject.GetComponent<MeshRenderer>();
            Material[] mts = renderer.materials;
            foreach (Material mt in mts)
            {
                mt.shader = Shader.Find(nameShader);

                if (mt.HasProperty("_Color"))
                {
                    Color prevColor = mt.GetColor("_Color");
                    mt.SetColor("_Color", new Color(prevColor.r, prevColor.g, prevColor.b, 0.5f));
                }
            }
            //    Debug.Log(other.name + "가려져라" );
            //    other.GetComponent<MeshRenderer>().material.color = new Color(0f, 0f, 0f, 0.3f);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Untagged"))
        {
            // changed to opaque
            //string nameShader = "Mobile/Unlit (Supports Lightmap)";
            string nameShader = "Standard";
            MeshRenderer renderer = other.gameObject.GetComponent<MeshRenderer>();
            Material[] mts = renderer.materials;
            foreach (Material mt in mts)
            {
                mt.shader = Shader.Find(nameShader);
            }
        }
    }

}
