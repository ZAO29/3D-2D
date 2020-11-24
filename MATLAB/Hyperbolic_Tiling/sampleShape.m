function sampled = sampleShape(vertices,nb_sample)

steps = diff(vertices,1,2)/(nb_sample-1);

steps2 = repmat(steps,[1,1,nb_sample]);
coeffs2 = repmat(reshape(0:(nb_sample-1),1,1,nb_sample),[2,nbVertex,1]);
vertices2 = repmat(vertices(:,1:(end-1)),1,1,nb_sample);
shape=steps2.*coeffs2+vertices2;
shape = permute(shape,[1,3,2]);
sampled = reshape(shape,[2,nbVertex*nb_sample]);


end

