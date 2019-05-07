using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAnimation : MonoBehaviour
{
    PlayerStatus player;
    Animator animator;

    // Start is called before the first frame update
    void Start()
    {
        player = GetComponent<PlayerStatus>();
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        MoveAni();
    }

    void MoveAni()
    {
        // 걷기
        if (player.is_Walk) animator.SetBool("walk", true);
        else animator.SetBool("walk", false);

        //뛰기
        if (player.isRun) animator.SetBool("run", true);
        else animator.SetBool("run", false);

    }
}
