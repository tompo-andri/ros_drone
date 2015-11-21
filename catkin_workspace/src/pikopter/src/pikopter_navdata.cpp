// Include pikopter navdata headers
#include "../include/pikopter/pikopter_navdata.h"

/***Mettre le sendto() dans la fonction chatterCallback **/
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("GOT MESSAGE FROM MAVLINK: [%s]", msg->data.c_str());
}

/*!
 * \brief Main function
 *
 * \param argc The number of arguments
 * \param argv The arguments
 */
int main(int argc, char **argv) {

	// We will use a PikopterNavdata object here
	PikopterNavdata pn;

	// Initialize ros for this node
	ros::init(argc, argv, "pikopter_navdata");

	// Create a node handle (fully initialize ros)
	ros::NodeHandle navdata_node_handle;
	ros::NodeHandle n;

	// Check the command syntax
	if (argc != 2) {
		ROS_FATAL("Command syntax is:\n \t%s \"ip_address\"\n", argv[0]);
		return -1;
	}

	// Put the rate for this node
	ros::Rate loop_rate(NAVDATA_LOOP_RATE);

	// Debug message
	ROS_DEBUG("Ros initialized with a rate of %u", NAVDATA_LOOP_RATE);

	// Open the UDP port for the navadata node
	pn.navdata_fd = PikopterNetwork::open_udp_socket(PORT_NAVDATA, &pn.addr_drone_navdata, argv[1]);

	// Get the length of the socket
	socklen_t len = sizeof(pn.addr_drone_navdata);

	// Then the main loop for the node
//	while(ros::ok()) {

		// Here we receive the navdatas from pikopter_mavlink
		 ros::Subscriber sub = n.subscribe("mavlink", 1000, chatterCallback);

		// Here we send it to jakopter
/*		if (sendto(pn.navdata_fd, pn.navdata_buffer, PACKET_SIZE, 0, (struct sockaddr*)&pn.addr_drone_navdata, sizeof(pn.addr_drone_navdata)) < 0) {
			ROS_ERROR("Error during sending a navdata packet");
		}
*/
		// Wait the next wake up
//		loop_rate.sleep();  // Normally, we just wait to receive a packet from pikopter_mavlink
//	}

	// Close the navdata fd at the end
//	if (pn.navdata_fd) close(pn.navdata_fd);

	/**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  	ros::spin();

	// Return the correct end status
	return 0;
}
