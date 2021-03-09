FROM amazonlinux
COPY smi1.sh .
RUN chmod ugo+x smi1.sh
CMD ./smi1.sh
