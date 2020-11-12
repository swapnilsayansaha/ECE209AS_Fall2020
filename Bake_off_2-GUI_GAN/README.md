# GUI-GAN: Towards an interactive graphical framework for privacy-preserving artificial data synthesis and imputation using generative adversarial networks.

In this project, we design a real-time and interactive graphical user interface (GUI) framework for synthesizing large time-series datasets from moderately-sized input datasets using Generative Adversarial Networks (GANs) called GUI-GAN. Specifically, we illustrate a generalizable graphical pipeline powered by a deep-learning backend that allows non-experts to generate synthetic sensory data while parameterizing synthetic data statistics via graphical tools. In addition, the framework allows the user to symbolically specify rectification statistics of generated data, which is then fed to another generative deep-learning pipeline for data imputation based on user-corrections.

For data synthesis and imputation, we evaluate an ensemble of two generative adversarial architectures:
* SenseGen - Generator: Stacked LSTM and Gaussian Mixture Model; Discriminator: LSTM https://ieeexplore.ieee.org/document/7917555 
* GAIN - we feed the input data, mask metadata characterizing missing data and a random matrix to the generator network, while the discriminator uses the generator output (imputed data) coupled with a hint matrix to try and distinguish between observed and imputed components. http://proceedings.mlr.press/v80/yoon18a/yoon18a.pdf 

Parts of the code have been borrowed from:
* https://github.com/nesl/sensegen
* https://github.com/jsyoon0823/GAIN

Datasets used in this project can be found at:
* https://data.mendeley.com/datasets/63zm778szb/3
* https://archive.ics.uci.edu/ml/machine-learning-databases/00240/ 

