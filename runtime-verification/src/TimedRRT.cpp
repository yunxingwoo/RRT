#include "TimedRRT.h"

TimedRRT::TimedRRT(string fileName): RRT(fileName){
}

TimedRRT::TimedRRT(int _d, int _k, int _var, double _simTime, string nam): RRT(_d+1, _k, _var, nam){ //Add time dimension to the RRT
    sim_time = _simTime;
    setBound(_d, 0, sim_time); //maximum time for sim, minimum is 0 and it is assigned in RRT constructor
}

TimedRRT::TimedRRT(int _d, int _k, int _var, string nam): RRT(_d+1, _k, _var, nam){ //Add time dimension to the RRT
    sim_time = 1 ; //default
    setBound(_d, 0, sim_time); //maximum time for sim, minimum is 0 and it is assigned in RRT constructor
}

void TimedRRT::build(double* initialState, double variation){
	double timeEnvlope=dt;		//time_envlope is the latest sampled discovered so far

	//first, we construct the root from the given initial state
	root = new node(d);
    root->set(initialState);
	root->setRoot();
	nodes.push_back(root);

	//main RRT loop
    for(int i=0;i<k; i++){
		cout <<"#################################################  Iteration  " << i << endl ;
        //create a new sample
        node* q_sample = new node(d);
        q_sample->randomize(min, max);
		//ensuring forward progress, initially we push the timeEnvlope to the simTime to gain depth, 
		//when we reached that time, we use it normally to ensure breath. 
		if (timeEnvlope >= sim_time){
			//double t = q_sample->unifRand(0, sim_time);
			q_sample->set(d - 1, -1);
		}else{
			q_sample->set(d - 1, timeEnvlope);
		}
		
        //find nearest node in the tree
        vector<node*> q_near_vec = getNearestNode(q_sample, -1, true);
		node* q_near = q_near_vec[0];
        double* state_near = q_near->get() ;
        double* state = new double[d];
        for(int j=0;j<d;j++){
			state[j]=state_near[j];
		}
        
        //variation or input to the system
        //todo: should be defined in the main or system, not here
		double* param = new double[1];
        param[0] = unifRand(-variation, variation);
        double t_init = state[d-1];
        double t = system->simulate(state, param, dt);
        state[d-1] = t_init + t ;
		cout << endl<< "Next state is " << state[0] << " " << state[1] << endl;
		if((state[d-1]>timeEnvlope) && (timeEnvlope<=sim_time)){ 
			cout << "Time envlope pushed to " << timeEnvlope << endl ;
			timeEnvlope=state[d-1];
		}

		node* q_new = new node(d);
        q_new->set(state);
        q_near->addChildren(q_new);		//add the new node to the tree
		q_new->setParent(q_near);		//We only make the parent-child releation ship during the tree build

		nodes.push_back(q_new);
		//casting
		//vector<node*> cast = getNearestNode(q_sample, 0.1, true);
		//cout << endl << "CAST size=" << cast.size() << endl ;
		//for(int j=0;j<cast.size();j++){
		//	cout << "Cast id = " << cast[j]->getID() << endl ;
		//	q_new->addCast(cast[j]);
		//	cast[j]->addCast(q_new);
		//}

		for(int i=0;i<monitors.size();i++){
			monitors[i]->check(q_new);
		}	
    }
}

void TimedRRT::addMonitor(Monitor* m){
	monitors.push_back(m);
}

double TimedRRT::getSimTime(){
	return sim_time;
}


void TimedRRT::simulate(double* initialState, double variation){
	root = new node(d);
    root->set(initialState);
	root->setRoot();
	nodes.push_back(root);
    for(int i=0;i<k; i++){
    	cout <<"#### " << i << endl ;
		node* q_near = nodes[ nodes.size()-1 ]; //last inserted node for the linear simulation
        double* state_near = q_near->get() ;
        double* state = new double[d];
        for(int j=0;j<d;j++){
			state[j]=state_near[j];
		}
        
        //variation or input to the system
        //todo: should be defined in the main or system, not here
		double* param = new double[1];
        param[0] = unifRand(-variation, variation);
        double t_init = state[d-1];
        double t = system->simulate(state, param, dt);
        state[d-1] = t_init + t ;
        node* q_new = new node(d);
        q_new->set(state);
        q_near->addChildren(q_new);		//add the new node to the tree
		q_new->setParent(q_near);		//We only make the parent-child releation ship during the tree build

		nodes.push_back(q_new);

		for(int i=0;i<monitors.size();i++){
			monitors[i]->check(q_new);
		}	
    }
}
