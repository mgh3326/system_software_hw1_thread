void main()
{
     thread_id tid;
     int arg;
     Init();

     thread_create(&tid, NULL, AppTask, &arg);

    RunScheduler();
     
}
