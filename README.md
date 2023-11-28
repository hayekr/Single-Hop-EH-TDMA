# Single Hop Energy Harvesting TDMA - SEHTDMDA

This repository contains the Arduino programs for implementing an energy harvesting wireless sensor network (WSN) using the TDMA (Time Division Multiple Access) algorithm. These programs were developed as part of a research paper conducted at Ohio Northern University. 

## Authors

- Robert Hayek, M.S.E.E Student, Northwestern University
  - ORCID: [0000-0002-4015-5897](https://orcid.org/0000-0002-4015-5897)
  - Email: robert.hayek@u.northwestern.edu

- Dr. Ahmed Ammar, Assistant Professor of Electrical & Computer Engineering, Ohio Northern University
  - ORCID: [0000-0001-5907-7043](https://orcid.org/0000-0001-5907-7043)
  - Email: a-ammar@onu.edu

## Affiliation

Ohio Northern University, Ada, Ohio, United States

## Research Paper

The research paper associated with this work can be found [here](https://ieeexplore.ieee.org/document/10316152).

## Poster Presentation

A poster presentation summarizing the research findings is available [here](https://digitalcommons.onu.edu/student_research_colloquium/2023/posters/44/)

## Abstract
Energy harvesting has been recently proposed for wireless sensor harvesting networks to improve their energy efficiency and drastically reduce the maintenance costs of replacing batteries. However, energy harvesting is a stochastic process which imposes an energy availability constraint on the system design. Thus, traditional protocols and algorithms including synchronization, communication, scheduling, and networking, will not be suitable for energy harvesting wireless networks. Therefore, this paper proposed a novel Time Division Multiplexing Algorithm for a single-hop energy harvesting wireless sensor network. The algorithm synchronizes the nodes based on less frequent synchronization messages from the coordinator node, which reduces power consumption and improves throughput. The algorithm was implemented in a network of four nodes; three nodes are senders, and one node is a coordinator.

The repository contains two Arduino programs:

1. **Sensor_Node**: This sketch is to be uploaded to eacg energy harvesting node on the network. It enables simulates energy harvesting and runs the TDMA algorithm.

2. **Coordinator_Node**: This sketch is to be uploaded to the base station node in the network. It acts as the central synchronization and error detection unit for the energy harvesting WSN.

## Setup and Usage

To use the programs in this repository, follow these steps:

1. Clone or download the repository to your local machine.

2. In **Coordinator_Node.ino** set time slot length and number of sensor nodes on the network variables.

3. In **Sensor_Node.ino** set the node ID number for each sensor node individually.

4. Upload the **Sensor_Node** sketch to the energy harvesting nodes.

5. Upload the **Coordinator_Node** sketch to the base station node.


## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code for academic and non-commercial purposes. However, please provide appropriate attribution to the authors when using this work.

If you have any questions or inquiries, please contact Robert Hayek (r-hayek@onu.edu) or Dr. Ahmed Ammar (a-ammar@onu.edu).
