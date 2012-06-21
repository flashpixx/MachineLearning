% ############################################################################
% # LGPL License                                                             #
% #                                                                          #
% # This file is part of the Machine Learning Framework.                     #
% # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
% # This program is free software: you can redistribute it and/or modify     #
% # it under the terms of the GNU Lesser General Public License as           #
% # published by the Free Software Foundation, either version 3 of the       #
% # License, or (at your option) any later version.                          #
% #                                                                          #
% # This program is distributed in the hope that it will be useful,          #
% # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
% # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
% # GNU Lesser General Public License for more details.                      #
% #                                                                          #
% # You should have received a copy of the GNU Lesser General Public License #
% # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
% ############################################################################


% plots a HDF file that is created by the "mds_nntp" example
% @param pcfile HDF file
function plotnntp( pcfile )
        lmarkersize=5;
 
        % create colors
        textlabel  = hdf5read( pcfile, '/uniquegroup');
        label      = cell(size(textlabel,1),1);
        labelcolor = cell(size(textlabel,1),1);
 
        col        = jet(size(textlabel,1));
        %col       = hsv(size(textlabel,1));
        for i=1:size(labelcolor,1)
            label{i}      = char(textlabel(i).data);
            labelcolor{i} = col(i, :);
        end
 
        % we create for each label group a data matrix
        data       = hdf5read( pcfile, '/project');
        if (size(data,2) ~= 2) && (size(data,2) ~= 3)
            error('plot only with 2D or 3D');
        end
 
        datalabel  = hdf5read( pcfile, '/group');
 
        datacell   = cell(size(label,1),1);
        maxcount   = zeros(size(label,1),1);
 
        for i=1:size(textlabel,1)
            datacell{i} = zeros( size(data,1), size(data,2) );
        end
 
        for i=1:size(data,1)
            pos   = strmatch(char(datalabel(i).data), label, 'exact');
            point = datacell{pos};
 
            point(maxcount(pos)+1,:) = data(i,:);
 
            datacell{pos} = point;
            maxcount(pos) = maxcount(pos) + 1;
        end
 
        % remove non-existing datasets
        emptycell = maxcount == 0;
        for i=1:numel(emptycell)
            if emptycell(i)
                datacell(i)   = [];
                labelcolor(i) = [];
                label(i)   = [];
                maxcount(i)   = [];
            end
        end
 
        % create plot
        figure;
        grid on;
        hold on;
 
        phandle = zeros(numel(datacell),1);
        for i=1:numel(datacell)
 
            point = datacell{i};
            point = point(1:maxcount(i), :);
 
            if size(point,2) == 2
                phandle(i) = plot(point(:,1), point(:,2), '.', 'Color', labelcolor{i}, 'DisplayName', label{i}, MarkerSize', lmarkersize);
            elseif size(point,2) == 3
                phandle(i) = plot3(point(:,1), point(:,2), point(:,3), '.', 'Color', labelcolor{i}, 'DisplayName', label{i}, 'MarkerSize', lmarkersize);
            end
        end
        set(gca,'fontsize',8);
        legend(phandle);
        
        if size(datacell{1},2) == 3
            view([-45 45]);
        end