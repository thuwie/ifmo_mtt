# Results
Results are presented in the `results` folder - [First](results/first.md), [Second](results/second.md), [Third](results/third.md)
# Structure
- `hadoop` - evn variables setters;
- `results` - contains the result;
- `settings` - master/worker spark settings;
- `src` - java 8 app;
- `target` - builded .jar;
- `.dockerignore`, `docker-compose.yml`, `Dockerfile` - docker stuff.

# How to run

1. `docker-compose up -d` - run containers
2. `docker exec -it ID /bin/bash` - access via ID of the master container
3. `cd ../../scripts` - go to the scripts directory. (Scripts are chmoded already)
- `standalone.sh` - star app in the standalone mode;
- `yarn.sh` - over yarn mode;
- `results.sh` - print results to the terminal;
- `stop.sh` - clear & exit.