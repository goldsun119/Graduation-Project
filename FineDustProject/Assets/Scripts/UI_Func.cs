using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_Func : MonoBehaviour
{
    GameObject playerObj;
    public PlayerStatus ps;

    private float start_pos = 0;
    
    //float h, w;
    // Start is called before the first frame update
    void Start()
    {
        //string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        //playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        //ps = playerObj.GetComponent<PlayerStatus>();
        start_pos = transform.position.y;
    }

    // Update is called once per frame
    void Update()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        ps = playerObj.GetComponent<PlayerStatus>();

        //Canvas canvas = FindObjectOfType<Canvas>();
        //h = canvas.GetComponent<RectTransform>().rect.height;

        
        if (ps.ID == Game.Network.NetWork.Client_id)
        {
            RectTransform Rect = transform.GetComponent<RectTransform>();
            Rect.localPosition = new Vector3(0, -(100 - ps.hp)/2, 0);
            Rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, 50); // 가로
            Rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, 50);   // 세로.
            //transform.Translate(Vector3.down * ps.hp / 2);
            //if ( Screen.height == 480)

            //    transform.position = new Vector3(transform.position.x, transform.parent.position.y - (100 - ps.hp), transform.position.z);

            //else
            //    transform.position = new Vector3(transform.position.x, transform.parent.position.y - ((float)(2 - (float)((float)((float)480 / Screen.height))) * (100 - ps.hp))  , transform.position.z);

            //Debug.Log((float)((float)480 / Screen.height));
            //Debug.Log((float)((float)((float)480 / Screen.height) * (100 - ps.hp)));
            //Debug.Log(Screen.width);
            //Debug.Log(Screen.height);
        }


    }
}
